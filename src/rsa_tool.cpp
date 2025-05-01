#include "rsa_tool.h"
#include <fstream>
#include <sstream>
#include <vector>

RSATool::RSATool() : m_rsa(nullptr) {
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
}

RSATool::~RSATool() {
    if (m_rsa) {
        RSA_free(m_rsa);
    }
    EVP_cleanup();
    ERR_free_strings();
}

void RSATool::clearError() {
    m_lastError.clear();
}

void RSATool::setError(const std::string& msg) {
    m_lastError = msg;
}

bool RSATool::generateKeyPair(int bits) {
    clearError();
    
    if (m_rsa) {
        RSA_free(m_rsa);
        m_rsa = nullptr;
    }

    // 生成RSA密钥对
    BIGNUM* bne = BN_new();
    if (!BN_set_word(bne, RSA_F4)) {
        setError("无法设置RSA指数");
        BN_free(bne);
        return false;
    }

    m_rsa = RSA_new();
    if (!RSA_generate_key_ex(m_rsa, bits, bne, nullptr)) {
        setError("生成RSA密钥对失败: " + std::string(ERR_error_string(ERR_get_error(), nullptr)));
        RSA_free(m_rsa);
        m_rsa = nullptr;
        BN_free(bne);
        return false;
    }

    BN_free(bne);
    return true;
}

bool RSATool::loadPublicKey(const std::string& pubKeyPath) {
    clearError();
    
    FILE* file = fopen(pubKeyPath.c_str(), "r");
    if (!file) {
        setError("无法打开公钥文件: " + pubKeyPath);
        return false;
    }

    if (m_rsa) {
        RSA_free(m_rsa);
        m_rsa = nullptr;
    }

    m_rsa = PEM_read_RSA_PUBKEY(file, nullptr, nullptr, nullptr);
    fclose(file);

    if (!m_rsa) {
        setError("读取公钥失败: " + std::string(ERR_error_string(ERR_get_error(), nullptr)));
        return false;
    }

    return true;
}

bool RSATool::loadPrivateKey(const std::string& privKeyPath) {
    clearError();
    
    FILE* file = fopen(privKeyPath.c_str(), "r");
    if (!file) {
        setError("无法打开私钥文件: " + privKeyPath);
        return false;
    }

    if (m_rsa) {
        RSA_free(m_rsa);
        m_rsa = nullptr;
    }

    m_rsa = PEM_read_RSAPrivateKey(file, nullptr, nullptr, nullptr);
    fclose(file);

    if (!m_rsa) {
        setError("读取私钥失败: " + std::string(ERR_error_string(ERR_get_error(), nullptr)));
        return false;
    }

    return true;
}

bool RSATool::savePublicKey(const std::string& pubKeyPath) {
    clearError();
    
    if (!m_rsa) {
        setError("没有可用的RSA密钥对");
        return false;
    }

    FILE* file = fopen(pubKeyPath.c_str(), "w");
    if (!file) {
        setError("无法创建公钥文件: " + pubKeyPath);
        return false;
    }

    if (PEM_write_RSA_PUBKEY(file, m_rsa) != 1) {
        setError("写入公钥失败: " + std::string(ERR_error_string(ERR_get_error(), nullptr)));
        fclose(file);
        return false;
    }

    fclose(file);
    return true;
}

bool RSATool::savePrivateKey(const std::string& privKeyPath) {
    clearError();
    
    if (!m_rsa) {
        setError("没有可用的RSA密钥对");
        return false;
    }

    FILE* file = fopen(privKeyPath.c_str(), "w");
    if (!file) {
        setError("无法创建私钥文件: " + privKeyPath);
        return false;
    }

    if (PEM_write_RSAPrivateKey(file, m_rsa, nullptr, nullptr, 0, nullptr, nullptr) != 1) {
        setError("写入私钥失败: " + std::string(ERR_error_string(ERR_get_error(), nullptr)));
        fclose(file);
        return false;
    }

    fclose(file);
    return true;
}

bool RSATool::signMessage(const std::string& message, std::string& signature) {
    clearError();
    
    if (!m_rsa) {
        setError("没有可用的RSA私钥");
        return false;
    }

    // 使用SHA-256哈希算法
    const EVP_MD* md = EVP_sha256();
    if (!md) {
        setError("不支持SHA-256哈希算法");
        return false;
    }

    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        setError("无法创建EVP上下文");
        return false;
    }

    // 初始化签名上下文
    if (EVP_DigestSignInit(mdctx, nullptr, md, nullptr, EVP_PKEY_new()) != 1) {
        setError("初始化签名上下文失败: " + std::string(ERR_error_string(ERR_get_error(), nullptr)));
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    // 设置RSA私钥
    EVP_PKEY* pkey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pkey, RSAPrivateKey_dup(m_rsa));
    EVP_MD_CTX_set_pkey_ctx(mdctx, EVP_PKEY_CTX_new(pkey, nullptr));

    // 更新消息
    if (EVP_DigestSignUpdate(mdctx, message.c_str(), message.length()) != 1) {
        setError("更新签名消息失败: " + std::string(ERR_error_string(ERR_get_error(), nullptr)));
        EVP_PKEY_free(pkey);
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    // 获取签名长度
    size_t siglen;
    if (EVP_DigestSignFinal(mdctx, nullptr, &siglen) != 1) {
        setError("获取签名长度失败: " + std::string(ERR_error_string(ERR_get_error(), nullptr)));
        EVP_PKEY_free(pkey);
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    // 分配签名缓冲区
    std::vector<unsigned char> sig(siglen);

    // 生成签名
    if (EVP_DigestSignFinal(mdctx, sig.data(), &siglen) != 1) {
        setError("生成签名失败: " + std::string(ERR_error_string(ERR_get_error(), nullptr)));
        EVP_PKEY_free(pkey);
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    // 将签名转换为Base64格式
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_write(bio, sig.data(), static_cast<int>(siglen));
    BIO_flush(bio);

    char* base64Data;
    long base64Length = BIO_get_mem_data(bio, &base64Data);
    signature.assign(base64Data, base64Length);

    // 清理资源
    BIO_free_all(bio);
    EVP_PKEY_free(pkey);
    EVP_MD_CTX_free(mdctx);

    return true;
}

bool RSATool::verifySignature(const std::string& message, const std::string& signature) {
    clearError();
    
    if (!m_rsa) {
        setError("没有可用的RSA公钥");
        return false;
    }

    // 使用SHA-256哈希算法
    const EVP_MD* md = EVP_sha256();
    if (!md) {
        setError("不支持SHA-256哈希算法");
        return false;
    }

    // 解码Base64签名
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* bio = BIO_new_mem_buf(signature.c_str(), static_cast<int>(signature.length()));
    bio = BIO_push(b64, bio);

    std::vector<unsigned char> sig(RSA_size(m_rsa));
    int siglen = BIO_read(bio, sig.data(), static_cast<int>(sig.size()));
    BIO_free_all(bio);

    if (siglen <= 0) {
        setError("解码Base64签名失败");
        return false;
    }

    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        setError("无法创建EVP上下文");
        return false;
    }

    // 初始化验证上下文
    if (EVP_DigestVerifyInit(mdctx, nullptr, md, nullptr, EVP_PKEY_new()) != 1) {
        setError("初始化验证上下文失败: " + std::string(ERR_error_string(ERR_get_error(), nullptr)));
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    // 设置RSA公钥
    EVP_PKEY* pkey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pkey, RSAPublicKey_dup(m_rsa));
    EVP_MD_CTX_set_pkey_ctx(mdctx, EVP_PKEY_CTX_new(pkey, nullptr));

    // 更新消息
    if (EVP_DigestVerifyUpdate(mdctx, message.c_str(), message.length()) != 1) {
        setError("更新验证消息失败: " + std::string(ERR_error_string(ERR_get_error(), nullptr)));
        EVP_PKEY_free(pkey);
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    // 验证签名
    int result = EVP_DigestVerifyFinal(mdctx, sig.data(), static_cast<size_t>(siglen));

    // 清理资源
    EVP_PKEY_free(pkey);
    EVP_MD_CTX_free(mdctx);

    if (result == 1) {
        return true;
    } else if (result == 0) {
        setError("签名验证失败");
        return false;
    } else {
        setError("验证过程中发生错误: " + std::string(ERR_error_string(ERR_get_error(), nullptr)));
        return false;
    }
}

std::string RSATool::getLastError() const {
    return m_lastError;
}
