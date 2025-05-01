#include "rsa_tool.h"
#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// 清除屏幕
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// 等待用户输入
void waitForUser() {
    std::cout << "\n按回车键继续...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// 主菜单
void showMenu() {
    clearScreen();
    std::cout << "========== RSA签名与验证工具 ==========\n";
    std::cout << "1. 生成RSA密钥对 (4096位)\n";
    std::cout << "2. 从文件加载密钥\n";
    std::cout << "3. 保存密钥到文件\n";
    std::cout << "4. 对消息签名\n";
    std::cout << "5. 验证签名\n";
    std::cout << "6. 退出\n";
    std::cout << "======================================\n";
    std::cout << "请选择操作 (1-6): ";
}

// 生成密钥对
void generateKeyPair(RSATool& rsaTool) {
    clearScreen();
    std::cout << "正在生成4096位RSA密钥对...\n";
    
    if (rsaTool.generateKeyPair(4096)) {
        std::cout << "密钥对生成成功！\n";
    } else {
        std::cout << "密钥对生成失败: " << rsaTool.getLastError() << "\n";
    }
    
    waitForUser();
}

// 从文件加载密钥
void loadKeyFromFile(RSATool& rsaTool) {
    clearScreen();
    std::cout << "1. 加载公钥\n";
    std::cout << "2. 加载私钥\n";
    std::cout << "请选择 (1-2): ";
    
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (choice < 1 || choice > 2) {
        std::cout << "无效选择！\n";
        waitForUser();
        return;
    }
    
    std::cout << "请输入密钥文件路径: ";
    std::string path;
    std::getline(std::cin, path);
    
    bool success = false;
    if (choice == 1) {
        success = rsaTool.loadPublicKey(path);
        if (success) {
            std::cout << "公钥加载成功！\n";
        } else {
            std::cout << "公钥加载失败: " << rsaTool.getLastError() << "\n";
        }
    } else {
        success = rsaTool.loadPrivateKey(path);
        if (success) {
            std::cout << "私钥加载成功！\n";
        } else {
            std::cout << "私钥加载失败: " << rsaTool.getLastError() << "\n";
        }
    }
    
    waitForUser();
}

// 保存密钥到文件
void saveKeyToFile(RSATool& rsaTool) {
    clearScreen();
    std::cout << "1. 保存公钥\n";
    std::cout << "2. 保存私钥\n";
    std::cout << "请选择 (1-2): ";
    
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (choice < 1 || choice > 2) {
        std::cout << "无效选择！\n";
        waitForUser();
        return;
    }
    
    std::cout << "请输入保存路径: ";
    std::string path;
    std::getline(std::cin, path);
    
    bool success = false;
    if (choice == 1) {
        success = rsaTool.savePublicKey(path);
        if (success) {
            std::cout << "公钥保存成功！\n";
        } else {
            std::cout << "公钥保存失败: " << rsaTool.getLastError() << "\n";
        }
    } else {
        success = rsaTool.savePrivateKey(path);
        if (success) {
            std::cout << "私钥保存成功！\n";
        } else {
            std::cout << "私钥保存失败: " << rsaTool.getLastError() << "\n";
        }
    }
    
    waitForUser();
}

// 对消息签名
void signMessage(RSATool& rsaTool) {
    clearScreen();
    std::cout << "请输入要签名的消息: ";
    
    std::string message;
    std::getline(std::cin, message);
    
    std::string signature;
    if (rsaTool.signMessage(message, signature)) {
        std::cout << "\n签名成功！\n";
        std::cout << "签名结果 (Base64编码):\n";
        std::cout << signature << "\n";
    } else {
        std::cout << "\n签名失败: " << rsaTool.getLastError() << "\n";
    }
    
    waitForUser();
}

// 验证签名
void verifySignature(RSATool& rsaTool) {
    clearScreen();
    std::cout << "请输入原始消息: ";
    std::string message;
    std::getline(std::cin, message);
    
    std::cout << "请输入签名 (Base64编码): ";
    std::string signature;
    std::getline(std::cin, signature);
    
    if (rsaTool.verifySignature(message, signature)) {
        std::cout << "\n签名验证通过！\n";
    } else {
        std::cout << "\n签名验证失败: " << rsaTool.getLastError() << "\n";
    }
    
    waitForUser();
}

int main() {
    RSATool rsaTool;
    int choice = 0;
    
    do {
        showMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                generateKeyPair(rsaTool);
                break;
            case 2:
                loadKeyFromFile(rsaTool);
                break;
            case 3:
                saveKeyToFile(rsaTool);
                break;
            case 4:
                signMessage(rsaTool);
                break;
            case 5:
                verifySignature(rsaTool);
                break;
            case 6:
                std::cout << "正在退出...\n";
                break;
            default:
                std::cout << "无效选择，请重新输入！\n";
                waitForUser();
                break;
        }
    } while (choice != 6);
    
    return 0;
}
