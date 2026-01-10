#include "../include/api_server.h"
#include "../include/audio_engine.h"
#include "../include/playlist_manager.h"
#include "../include/database_manager.h"
#include <iostream>
#include <csignal>
#include <atomic>
#include <thread>
#include <chrono>

using namespace musicfree;

std::atomic<bool> should_exit(false);

void signalHandler(int signal) {
    (void)signal;  // Unused parameter
    std::cout << "\nShutting down..." << std::endl;
    should_exit = true;
}

int main(int argc, char* argv[]) {
    std::cout << "================================================" << std::endl;
    std::cout << "  MusicFree Desktop - Backend Server v0.1.0   " << std::endl;
    std::cout << "================================================" << std::endl;
    std::cout << std::endl;

    // 设置信号处理
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    // 启动 API 服务器
    ApiServer api_server;
    
    int port = 8888;
    if (argc > 1) {
        port = std::stoi(argv[1]);
    }

    std::cout << "Starting API server on port " << port << "..." << std::endl;
    
    if (!api_server.start(port)) {
        std::cerr << "Failed to start API server!" << std::endl;
        return 1;
    }

    std::cout << "[OK] API server started successfully" << std::endl;
    std::cout << "Base URL: http://127.0.0.1:" << port << std::endl;
    std::cout << std::endl;
    std::cout << "API Documentation:" << std::endl;
    std::cout << "  POST   /api/player/load       - Load audio file" << std::endl;
    std::cout << "  POST   /api/player/play       - Play" << std::endl;
    std::cout << "  POST   /api/player/pause      - Pause" << std::endl;
    std::cout << "  POST   /api/player/stop       - Stop" << std::endl;
    std::cout << "  GET    /api/player/status    - Get player status" << std::endl;
    std::cout << "  GET    /api/playlist         - Get current playlist" << std::endl;
    std::cout << "  GET    /api/search?q=keyword - Search music" << std::endl;
    std::cout << std::endl;
    std::cout << "Press Ctrl+C to exit..." << std::endl;
    std::cout << std::endl;

    // 保持运行直到收到退出信号
    while (!should_exit) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Stopping API server..." << std::endl;
    api_server.stop();
    std::cout << "[OK] Server stopped" << std::endl;

    return 0;
}
