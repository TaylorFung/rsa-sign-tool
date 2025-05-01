#ifndef RSA_TOOL_H
#define RSA_TOOL_H

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <string>

/**
 * @brief RSA工具类，提供密钥生成、签名和验证功能
 */
class RSATool {
public:
    RSATool();
    ~RSATool();

    /**
     * @brief 生成RSA密钥对
     * @param bits 密钥长度
     * @return 是否生成成功
     */
    bool generateKeyPair(int bits = 4096);

    /**
     * @brief 从文件加载公钥
     * @param pubKeyPath 公钥文件路径
     * @return 是否加载成功
     */
    bool loadPublicKey(const std::string& pubKeyPath);

    /**
     * @brief 从文件加载私钥
     * @param privKeyPath 私钥文件路径
     * @return 是否加载成功
     */
    bool loadPrivateKey(const std::string& privKeyPath);

    /**
     * @brief 保存公钥到文件
     * @param pubKeyPath 公钥文件路径
     * @return 是否保存成功
     */
    bool savePublicKey(const std::string& pubKeyPath);

    /**
     * @brief 保存私钥到文件
     * @param privKeyPath 私钥文件路径
     * @return 是否保存成功
     */
    bool savePrivateKey(const std::string& privKeyPath);

    /**
     * @brief 对消息进行签名
     * @param message 要签名的消息
     * @param signature 输出签名结果
     * @return 是否签名成功
     */
    bool signMessage(const std::string& message, std::string& signature);

    /**
     * @brief 验证签名
     * @param message 原始消息
     * @param signature 签名
     * @return 验证是否通过
     */
    bool verifySignature(const std::string& message, const std::string& signature);

    /**
     * @brief 获取错误信息
     * @return 最后的错误信息
     */
    std::string getLastError() const;

private:
    RSA* m_rsa;                 // RSA密钥对
    std::string m_lastError;    // 最后的错误信息

    /**
     * @brief 清除错误信息
     */
    void clearError();

    /**
     * @brief 设置错误信息
     * @param msg 错误信息
     */
    void setError(const std::string& msg);
};

#endif // RSA_TOOL_H
