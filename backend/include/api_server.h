#ifndef MUSICFREE_API_SERVER_H
#define MUSICFREE_API_SERVER_H

#include <string>
#include <memory>

namespace musicfree {

/**
 * REST API 服务器
 * 向前端提供 HTTP 接口
 */
class ApiServer {
public:
    ApiServer();
    ~ApiServer();

    // 禁止拷贝
    ApiServer(const ApiServer&) = delete;
    ApiServer& operator=(const ApiServer&) = delete;

    /**
     * 启动 API 服务器
     * @param port HTTP 服务端口
     * @return 成功返回 true
     */
    bool start(int port = 8888);

    /**
     * 停止 API 服务器
     */
    void stop();

    /**
     * 获取服务器是否运行中
     * @return 运行中返回 true
     */
    bool isRunning() const;

    /**
     * 获取服务器绑定的端口
     * @return 端口号
     */
    int getPort() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace musicfree

#endif  // MUSICFREE_API_SERVER_H
