# HTTP / HTTPS

HTTP 和 HTTPS 是我们每天都在使用的应用层协议。浏览器打开网页、手机 App 调接口、嵌入式设备上传数据、开发板提供配置页面，背后都可能是 HTTP 或 HTTPS。

初学时要先分清：HTTP 不是“网页本身”，而是一套客户端和服务器交换请求、响应的应用层协议；HTTPS 也不是“更高级的 HTTP”，而是 HTTP 运行在 TLS 安全层之上，解决加密、完整性和身份认证问题。

## 1. HTTP / HTTPS 概述

HTTP 是 Hypertext Transfer Protocol，超文本传输协议。它最初用于传输超文本页面，现在已经广泛用于 Web API、文件下载、设备配置、云平台通信等场景。

HTTP 的核心模型很简单：

```text
客户端发送请求 -> 服务器返回响应
```

例如：

- 浏览器请求 HTML、CSS、JavaScript、图片。
- App 请求后端接口，获得 JSON。
- 嵌入式设备向云平台 POST 传感器数据。
- PC 通过 HTTP 页面配置开发板参数。

HTTPS 是 HTTP Secure。常见理解是：

```text
HTTPS = HTTP + TLS
```

TLS 位于 HTTP 和 TCP 之间，为 HTTP 数据提供：

- 加密：别人抓到包也难以直接读出内容。
- 完整性：防止数据被中途篡改而不被发现。
- 身份认证：客户端可以验证服务器证书，确认自己连接的是目标域名对应的服务器。

## 2. HTTP 在网络分层中的位置

HTTP 是应用层协议。它不直接控制网线、MAC 地址或 IP 路由，而是依赖下层协议完成传输。

<figure markdown="span">
  <img src="../assets/http-https/http-layering.svg" alt="HTTP and HTTPS in network stack" />
  <figcaption>图 1：HTTP / HTTPS 在网络协议栈中的位置。</figcaption>
</figure>

常见分层关系：

| 层次 | HTTP | HTTPS |
| --- | --- | --- |
| 应用层 | HTTP | HTTP |
| 安全层 | 无 | TLS |
| 传输层 | TCP | TCP |
| 网络层 | IP | IP |
| 链路层 | Ethernet / Wi-Fi | Ethernet / Wi-Fi |

HTTP/3 比较特殊，它不是运行在 TCP 上，而是运行在 QUIC 上；QUIC 又运行在 UDP 上。初学阶段可以先记住：

- HTTP/1.1 和 HTTP/2 通常基于 TCP。
- HTTPS 常见是 HTTP + TLS + TCP。
- HTTP/3 是 HTTP 语义运行在 QUIC/UDP 上。

## 3. HTTP 与 TCP/IP 的关系

HTTP 关心的是“请求什么资源、返回什么状态、Body 是什么格式”。TCP/IP 关心的是“数据如何从一个进程可靠送到另一个进程”。

例如访问：

```text
https://example.com/api/status
```

大致会发生：

1. DNS 把 `example.com` 解析成 IP 地址。
2. 客户端与服务器建立 TCP 连接，通常端口是 `443`。
3. 客户端与服务器进行 TLS 握手，建立安全通道。
4. 客户端在 TLS 通道里发送 HTTP 请求。
5. 服务器返回 HTTP 响应。

HTTP 本身不负责：

- IP 寻址。
- TCP 重传。
- 拥塞控制。
- 以太网帧转发。
- TLS 证书校验。

这些由下层协议和运行环境完成。

## 4. URL 的基本组成

URL 是 Uniform Resource Locator，统一资源定位符。它描述要访问什么资源。

例子：

```text
https://user:pass@example.com:8443/api/v1/data?dev=1&page=2#section
```

常见组成：

| 部分 | 示例 | 说明 |
| --- | --- | --- |
| scheme | `https` | 使用的协议或方案。 |
| userinfo | `user:pass` | 用户信息，现代 Web 中较少使用。 |
| host | `example.com` | 域名或 IP。 |
| port | `8443` | 端口，省略时使用默认端口。 |
| path | `/api/v1/data` | 资源路径。 |
| query | `dev=1&page=2` | 查询参数。 |
| fragment | `section` | 片段标识，通常只在客户端使用，不会随 HTTP 请求发给服务器。 |

默认端口：

- HTTP 默认端口 `80`。
- HTTPS 默认端口 `443`。

URL 中的 host 会出现在 HTTP/1.1 的 `Host` 请求头中。HTTPS 场景下，TLS 握手还常用 SNI 告诉服务器客户端想访问哪个域名。

## 5. 请求/响应模型

HTTP 是典型请求/响应协议。

<figure markdown="span">
  <img src="../assets/http-https/http-message-structure.svg" alt="HTTP request and response structure" />
  <figcaption>图 2：HTTP 请求与响应结构。</figcaption>
</figure>

请求由客户端发出，响应由服务器返回。一次典型访问如下：

<figure markdown="span">
  <img src="../assets/http-https/http-access-flow.svg" alt="Typical HTTP access flow" />
  <figcaption>图 3：一次典型 HTTP 访问流程。</figcaption>
</figure>

HTTP 报文通常包含：

- 起始行。
- Header 字段。
- 空行。
- 可选 Body。

请求示例：

```http
GET /api/status HTTP/1.1
Host: example.com
User-Agent: curl/8.0
Accept: application/json

```

响应示例：

```http
HTTP/1.1 200 OK
Content-Type: application/json
Content-Length: 17
Cache-Control: no-store

{"status":"ok"}
```

注意：HTTP/2 和 HTTP/3 在传输层面的编码不再是这种纯文本起始行形式，但语义上仍然有方法、路径、状态码、头字段和内容。

## 6. 常见请求方法

HTTP 方法表示客户端希望对资源执行什么操作。

| 方法 | 常见含义 | 是否常带 Body | 典型用途 |
| --- | --- | --- | --- |
| GET | 获取资源 | 通常不带 | 查询页面、读取数据。 |
| POST | 提交数据或触发处理 | 常带 | 登录、提交表单、创建业务动作。 |
| PUT | 创建或整体替换资源 | 常带 | 更新完整配置。 |
| PATCH | 局部修改资源 | 常带 | 修改资源的部分字段。 |
| DELETE | 删除资源 | 通常不带或少量 | 删除某个对象。 |
| HEAD | 只获取响应头 | 不带 | 检查资源是否存在、获取长度。 |
| OPTIONS | 查询支持能力 | 可选 | CORS 预检、能力发现。 |

常见误区：

- GET 不应依赖 Body。很多工具允许 GET 带 Body，但兼容性和语义都不好。
- GET 应尽量设计成安全、幂等，不应触发危险修改。
- PUT 通常表示整体替换，PATCH 通常表示局部修改。
- POST 不一定只能用于创建，也常用于复杂查询或动作触发，但接口设计时要讲清楚语义。

## 7. 请求报文结构

HTTP/1.1 请求报文结构：

```text
请求行
请求头
空行
请求体
```

请求行示例：

```http
POST /api/device/report HTTP/1.1
```

包含：

- Method：`POST`。
- Request Target：`/api/device/report`。
- HTTP Version：`HTTP/1.1`。

请求头示例：

```http
Host: iot.example.com
Content-Type: application/json
Authorization: Bearer eyJhbGciOi...
Content-Length: 35
```

请求体示例：

```json
{"temperature":25.6,"humidity":60}
```

嵌入式设备发送 HTTP 请求时，要特别注意：

- Header 和 Body 之间必须有空行。
- `Content-Length` 要与 Body 字节数一致。
- 文本长度和字节长度不一定相同，中文 UTF-8 会占多个字节。
- TCP 是字节流，不能假设一次发送就是服务器一次完整读取。

## 8. 响应报文结构

HTTP/1.1 响应报文结构：

```text
状态行
响应头
空行
响应体
```

状态行示例：

```http
HTTP/1.1 404 Not Found
```

包含：

- HTTP Version：`HTTP/1.1`。
- Status Code：`404`。
- Reason Phrase：`Not Found`，HTTP/2 之后不再依赖原因短语。

响应头示例：

```http
Content-Type: text/html; charset=utf-8
Cache-Control: max-age=3600
Set-Cookie: sid=abc; HttpOnly; Secure; SameSite=Lax
```

响应体可以是：

- HTML。
- JSON。
- 图片。
- 文件。
- 空内容。

## 9. 状态码分类与典型示例

HTTP 状态码表示服务器对请求的处理结果。

| 分类 | 范围 | 含义 |
| --- | --- | --- |
| 1xx | `100..199` | 信息性响应。 |
| 2xx | `200..299` | 成功。 |
| 3xx | `300..399` | 重定向。 |
| 4xx | `400..499` | 客户端错误。 |
| 5xx | `500..599` | 服务器错误。 |

常见状态码：

| 状态码 | 名称 | 说明 |
| --- | --- | --- |
| 200 | OK | 请求成功。 |
| 201 | Created | 资源创建成功。 |
| 204 | No Content | 成功但无响应体。 |
| 301 | Moved Permanently | 永久重定向。 |
| 302 | Found | 临时重定向，历史兼容语义较复杂。 |
| 304 | Not Modified | 缓存资源未修改。 |
| 400 | Bad Request | 请求格式错误。 |
| 401 | Unauthorized | 未认证或认证失败。 |
| 403 | Forbidden | 已理解请求，但拒绝访问。 |
| 404 | Not Found | 资源不存在。 |
| 405 | Method Not Allowed | 方法不允许。 |
| 408 | Request Timeout | 请求超时。 |
| 409 | Conflict | 资源状态冲突。 |
| 413 | Content Too Large | 请求体过大。 |
| 429 | Too Many Requests | 请求过多，被限流。 |
| 500 | Internal Server Error | 服务器内部错误。 |
| 502 | Bad Gateway | 网关从上游收到无效响应。 |
| 503 | Service Unavailable | 服务不可用。 |
| 504 | Gateway Timeout | 网关等待上游超时。 |

排障时要区分：

- 4xx 通常先查客户端请求、权限、路径、参数。
- 5xx 通常先查服务器、网关、后端服务、日志。
- 没有 HTTP 状态码，说明可能还没到 HTTP 层，例如 DNS、TCP、TLS 已经失败。

## 10. 常见请求头与响应头

HTTP Header 是键值形式的元数据。

### 10.1 Host

```http
Host: example.com
```

HTTP/1.1 请求必须包含 Host。一个服务器 IP 上可能托管多个域名，服务器通过 Host 判断要访问哪个站点。

### 10.2 Content-Type

```http
Content-Type: application/json
Content-Type: text/html; charset=utf-8
Content-Type: application/x-www-form-urlencoded
Content-Type: multipart/form-data; boundary=...
```

Content-Type 表示 Body 的媒体类型。客户端和服务器都依赖它解析数据。

常见问题：

- JSON 接口忘记设置 `application/json`。
- 表单和 JSON 混用。
- 字符集不一致导致中文乱码。

### 10.3 Content-Length 与 Transfer-Encoding

`Content-Length` 表示 Body 字节数。

```http
Content-Length: 123
```

`Transfer-Encoding: chunked` 表示分块传输，常见于 HTTP/1.1 动态响应。

嵌入式 HTTP Server 如果不支持 chunked，要明确使用 Content-Length 或关闭连接作为响应结束标志。

### 10.4 Authorization

Authorization 用于携带认证信息。

```http
Authorization: Basic ...
Authorization: Bearer <token>
```

Bearer Token 常用于 API 访问。注意不要在日志中明文打印完整 Token。

### 10.5 Cookie 与 Set-Cookie

客户端请求时带 Cookie：

```http
Cookie: sid=abc; theme=dark
```

服务器响应时设置 Cookie：

```http
Set-Cookie: sid=abc; HttpOnly; Secure; SameSite=Lax
```

Cookie 常用于浏览器会话、登录状态、偏好设置等。

### 10.6 Cache-Control

Cache-Control 控制缓存策略。

```http
Cache-Control: no-store
Cache-Control: max-age=3600
Cache-Control: public, max-age=86400
```

常见含义：

- `no-store`：不要存储响应。
- `no-cache`：可以存储，但使用前必须重新验证。
- `max-age=3600`：缓存 3600 秒。
- `public`：可被共享缓存保存。
- `private`：只适合用户私有缓存。

缓存可以显著提升性能，但接口设计不当也可能导致旧数据、权限数据泄露或调试困惑。

### 10.7 User-Agent、Accept、Location

`User-Agent` 表示客户端信息。

`Accept` 表示客户端希望接收的内容类型。

`Location` 常用于重定向或创建资源后的新地址。

```http
HTTP/1.1 302 Found
Location: /login
```

## 11. Cookie、Session、Token 的区别

这三个概念经常混在一起。

| 概念 | 存在哪里 | 用途 | 典型形式 |
| --- | --- | --- | --- |
| Cookie | 浏览器本地，由浏览器自动随请求携带 | 保存少量状态或会话标识 | `sid=abc` |
| Session | 服务器端 | 保存某个用户的登录状态和上下文 | 服务器内存、Redis、数据库 |
| Token | 客户端持有，服务端验证 | API 身份凭证或授权凭证 | JWT、随机字符串 |

典型 Cookie + Session 流程：

1. 用户登录。
2. 服务器创建 Session。
3. 服务器通过 `Set-Cookie` 返回 `sid`。
4. 浏览器后续请求自动带 `Cookie: sid=...`。
5. 服务器用 `sid` 找到对应 Session。

典型 Token 流程：

1. 用户登录。
2. 服务器返回 Token。
3. 客户端保存 Token。
4. 后续请求带 `Authorization: Bearer <token>`。
5. 服务器验证 Token。

注意：

- Cookie 是一种浏览器存储和自动携带机制，不等于 Session。
- Session 通常在服务器端，Cookie 里可能只存 Session ID。
- Token 不一定存 Cookie，也可以存在 App 本地存储或内存里。
- Token 泄露同样危险，HTTPS 和安全存储仍然重要。

## 12. HTTP/1.1、HTTP/2、HTTP/3 简要区别

| 版本 | 传输基础 | 主要特点 |
| --- | --- | --- |
| HTTP/1.1 | TCP | 文本报文，持久连接，管线化理论存在但实践受限。 |
| HTTP/2 | TCP | 二进制分帧，多路复用，头部压缩，服务器推送曾被设计但实际使用减少。 |
| HTTP/3 | QUIC over UDP | 基于 QUIC，减少 TCP 队头阻塞影响，连接迁移能力更好。 |

HTTP/2 的多路复用可以在一个 TCP 连接上并发多个请求响应流，但因为底层仍是 TCP，如果 TCP 层丢包，所有流都可能受影响。

HTTP/3 运行在 QUIC 上，QUIC 使用 UDP 作为承载，并在用户态实现连接、加密、流、多路复用、丢包恢复等能力。初学阶段不要理解成“HTTP/3 就是不可靠的 UDP”。HTTP/3 只是使用 UDP 穿过网络，可靠性和安全性由 QUIC 提供。

## 13. HTTPS 是什么

HTTPS 是在 TLS 安全通道中传输 HTTP。

<figure markdown="span">
  <img src="../assets/http-https/http-https-tcp-tls.svg" alt="HTTP HTTPS TCP TLS relationship" />
  <figcaption>图 4：HTTP、HTTPS、TCP、TLS 的关系。</figcaption>
</figure>

HTTP 明文传输存在几个问题：

- 窃听：中间人可以看到请求路径、Header、Body。
- 篡改：中间人可能修改响应内容。
- 冒充：客户端无法可靠确认服务器身份。

HTTPS 用 TLS 解决这些问题：

- 用加密保护内容。
- 用消息认证和完整性校验发现篡改。
- 用证书链验证服务器身份。

HTTPS 不等于绝对安全。它保护传输通道，但不能自动解决：

- 服务器漏洞。
- 弱密码。
- XSS、CSRF。
- Token 泄露。
- 客户端本地被攻破。
- 错误信任证书。

## 14. TLS 的基本作用

TLS 是 Transport Layer Security，传输层安全协议。它常位于应用层和传输层之间。

TLS 主要提供三类能力：

### 14.1 加密

通信内容经过加密，中间设备即使抓到包，也难以直接读出 HTTP 请求和响应内容。

### 14.2 完整性

TLS 会检查数据是否被篡改。如果中间人修改密文，接收方通常能发现并终止连接。

### 14.3 身份认证

服务器提供证书，客户端验证证书链、有效期、域名等，以确认连接的是目标服务器。

某些场景还会使用双向 TLS，服务器也验证客户端证书。这常见于企业、工业网关、设备到云平台的高安全接入。

## 15. 证书、CA、域名校验

HTTPS 服务器证书通常包含：

- 证书持有者信息。
- 公钥。
- 域名信息，现代证书主要看 Subject Alternative Name。
- 有效期。
- 签发者。
- 签名算法和签名。

CA 是 Certificate Authority，证书颁发机构。浏览器和操作系统内置一批受信任根 CA。服务器证书通常由中间 CA 签发，最终能追溯到受信任根 CA。

客户端验证证书时通常检查：

- 证书链是否能连到受信任根 CA。
- 证书是否在有效期内。
- 证书是否被吊销，具体机制和客户端实现有关。
- 访问的域名是否匹配证书中的域名。
- 证书签名算法是否安全。

常见证书错误：

- 证书过期。
- 域名不匹配。
- 自签名证书未被信任。
- 中间证书缺失。
- 设备时间错误导致证书看起来未生效或已过期。
- 嵌入式设备根证书库太旧。

## 16. HTTPS 建立连接的大致流程

HTTPS 建立连接包含 TCP 握手和 TLS 握手。下面是简化流程。

<figure markdown="span">
  <img src="../assets/http-https/https-tls-flow.svg" alt="HTTPS TLS handshake simplified flow" />
  <figcaption>图 5：HTTPS / TLS 建立安全连接的简化流程。</figcaption>
</figure>

典型流程：

1. 客户端与服务器建立 TCP 连接。
2. 客户端发送 TLS ClientHello，包含支持的 TLS 版本、加密套件、随机数、SNI 等。
3. 服务器返回 ServerHello，选择参数，并发送证书等信息。
4. 客户端验证证书链、有效期和域名。
5. 双方协商出会话密钥。
6. 后续 HTTP 数据使用协商出的密钥加密传输。

这是为了帮助初学者理解的简化说法。TLS 1.2 和 TLS 1.3 的握手细节不同，密钥交换、消息数量、0-RTT 等机制也不同。工程上通常使用成熟 TLS 库，例如 OpenSSL、mbedTLS、wolfSSL、BoringSSL，而不是自己实现 TLS。

## 17. 调试方法

### 17.1 浏览器开发者工具

浏览器 DevTools 的 Network 面板可以查看：

- URL。
- 方法。
- 状态码。
- 请求头和响应头。
- Query 参数。
- Request Payload。
- Response Body。
- Cookie。
- 缓存命中情况。
- Timing。

适合调试网页、接口、CORS、缓存和 Cookie。

### 17.2 curl

`curl` 是命令行调试 HTTP/HTTPS 的常用工具。

```bash
curl -v https://example.com/
curl -X POST https://api.example.com/data -H "Content-Type: application/json" -d "{\"a\":1}"
curl -I https://example.com/
```

常用选项：

- `-v`：显示连接和 TLS/HTTP 细节。
- `-I`：只请求响应头。
- `-H`：添加 Header。
- `-d`：发送请求体。
- `--cacert`：指定 CA 证书。
- `-k`：跳过证书校验，仅调试时使用，不应在生产中使用。

### 17.3 Postman / Apifox 等 API 工具

适合调试 REST API：

- 管理环境变量。
- 设置 Header。
- 设置 Bearer Token。
- 查看响应体。
- 保存接口集合。

### 17.4 Wireshark

Wireshark 可以抓 HTTP 明文流量，也可以看到 HTTPS 的 TCP/TLS 握手信息。

HTTPS 内容默认是加密的，抓包通常看不到 HTTP Header 和 Body。但仍能看到：

- DNS。
- TCP 握手。
- TLS 版本。
- SNI，具体是否可见取决于是否使用 ECH 等新机制。
- 证书信息，TLS 版本和配置会影响可见性。
- TCP 重传、RST、握手失败。

调试 HTTPS 内容时，可以使用浏览器开发者工具、服务器日志、客户端日志，或在受控环境中配置 TLS key log。不要在生产环境随意泄露密钥日志。

## 18. 常见错误与排障思路

### 18.1 没有 HTTP 状态码

如果客户端没有拿到 HTTP 状态码，问题可能发生在更低层：

- DNS 解析失败。
- TCP 连接失败。
- TLS 握手失败。
- 代理或防火墙阻断。
- 连接超时。

排查顺序：

1. DNS 是否能解析。
2. IP 是否能连通。
3. TCP 端口是否开放。
4. TLS 证书是否通过校验。
5. HTTP 请求是否发出。

### 18.2 400 Bad Request

常见原因：

- URL、Query、Header 格式错误。
- JSON 不合法。
- Content-Length 错误。
- Content-Type 与 Body 不匹配。
- 服务端参数校验失败。

### 18.3 401 与 403

`401 Unauthorized` 通常表示认证缺失或失败。

`403 Forbidden` 通常表示身份可能已确认，但没有权限访问。

检查：

- Authorization 是否带上。
- Token 是否过期。
- Cookie 是否被浏览器阻止。
- 权限角色是否正确。
- 系统时间是否影响 Token 或证书。

### 18.4 404

常见原因：

- URL 路径错误。
- HTTP 方法错误。
- 反向代理路由错误。
- 服务部署路径和接口文档不一致。

### 18.5 5xx

常见原因：

- 后端程序异常。
- 数据库不可用。
- 上游服务超时。
- 网关配置错误。
- 服务过载。

客户端看到 5xx 时，不应盲目高频重试，否则可能加重服务器压力。

### 18.6 HTTPS 证书错误

常见原因：

- 证书过期。
- 域名不匹配。
- 根 CA 不受信任。
- 中间证书链缺失。
- 设备时间错误。
- 嵌入式固件证书库太旧。

嵌入式设备尤其容易因为 RTC 时间不准导致 TLS 证书校验失败。很多设备需要先同步时间，再建立 HTTPS 连接。

## 19. 工程实践建议

### 19.1 接口设计

- URL 表达资源，方法表达动作。
- GET 用于读取，POST/PUT/PATCH/DELETE 用于修改。
- 返回一致的 JSON 结构。
- 错误响应包含可读错误码和信息。
- 对请求体大小做限制。
- 对接口版本进行规划，例如 `/api/v1/...`。

### 19.2 安全意识

- 生产环境使用 HTTPS。
- 不在 URL Query 中放敏感 Token。
- Token 不要明文打印到日志。
- Cookie 设置 `HttpOnly`、`Secure`、`SameSite`。
- 管理接口增加认证和权限控制。
- 嵌入式设备避免使用硬编码万能密码。
- 证书校验不要在生产中关闭。

### 19.3 缓存

- 静态资源可以设置较长缓存。
- 用户私有数据使用 `private` 或 `no-store`。
- 配置接口和实时数据谨慎缓存。
- 使用 ETag 或 Last-Modified 做条件请求。

### 19.4 超时与重试

客户端应区分：

- DNS 超时。
- TCP 连接超时。
- TLS 握手超时。
- 请求发送超时。
- 响应读取超时。

重试建议：

- GET 等幂等请求更适合自动重试。
- POST 是否能重试取决于业务是否幂等。
- 使用指数退避，避免雪崩。
- 对 429、503 可结合 `Retry-After`。

### 19.5 嵌入式实现

嵌入式设备做 HTTP/HTTPS 时要关注：

- RAM 是否足够容纳 TLS 缓冲区和证书链。
- Flash 是否足够存根证书。
- RTC 时间是否正确。
- 是否支持 SNI。
- 是否需要 HTTP chunked。
- 是否需要长连接。
- 网络断开后是否能自动重连。
- OTA 下载是否校验固件签名，不要只依赖 HTTPS。

实现方式与平台强相关。裸机、RTOS、Linux、蜂窝模组 AT 指令、Wi-Fi 模组 SDK 的 HTTP/HTTPS 能力差异很大，要看对应官方文档。

## 20. 与 TCP/UDP、WebSocket、MQTT 的对比

| 协议 | 层次/定位 | 常见承载 | 特点 |
| --- | --- | --- | --- |
| TCP | 传输层 | IP | 可靠字节流，HTTP/1.1/2 常用。 |
| UDP | 传输层 | IP | 无连接数据报，HTTP/3 的 QUIC 使用 UDP。 |
| HTTP | 应用层 | TCP | 请求/响应模型，Web API 常用。 |
| HTTPS | 应用层 + TLS | TCP | HTTP 通过 TLS 加密传输。 |
| WebSocket | 应用层 | TCP，常由 HTTP Upgrade 建立 | 长连接、双向消息。 |
| MQTT | 应用层 | TCP/TLS | 发布订阅模型，物联网常用。 |

边界理解：

- HTTP 适合请求/响应。
- WebSocket 适合浏览器和服务器之间长期双向推送。
- MQTT 适合设备与 Broker 的发布订阅。
- TCP/UDP 是更底层的传输能力，不定义业务资源和状态码。

## 21. 常见误区

### 21.1 HTTP 是网页本身

不是。网页是 HTML、CSS、JavaScript、图片等资源；HTTP 是传输这些资源的协议。

### 21.2 HTTPS 是更高级的 HTTP

不准确。HTTPS 是 HTTP over TLS。HTTP 语义仍然是请求、响应、方法、状态码、头字段，只是传输通道被 TLS 保护。

### 21.3 Cookie、Session、Token 是同一个东西

不是。Cookie 是客户端存储和自动携带机制；Session 多指服务器端会话状态；Token 是访问凭证，常通过 Authorization 或 Cookie 携带。

### 21.4 HTTP/3 使用 UDP，所以不可靠

不准确。HTTP/3 使用 QUIC，QUIC 基于 UDP，但自己实现了可靠传输、加密、多路复用和拥塞控制等机制。它不是简单把 HTTP 文本直接塞进 UDP。

### 21.5 HTTPS 抓不到明文就无法调试

不是。可以用浏览器开发者工具、客户端日志、服务器日志、受控环境 TLS key log 等方式调试。Wireshark 仍能帮助分析 DNS、TCP、TLS 握手和连接问题。

## 22. 学习路线

建议按下面顺序练习：

1. 用浏览器 DevTools 打开一个网页，观察请求方法、状态码、Header。
2. 用 `curl -v` 请求 HTTP 和 HTTPS 网站，对比连接过程。
3. 手写一个简单 HTTP 请求，理解请求行、Header、空行。
4. 用本地 HTTP Server 返回 JSON，观察 Content-Type。
5. 设计一个 REST 风格接口，区分 GET、POST、PUT、DELETE。
6. 使用 Cookie 登录一个测试页面，观察 Set-Cookie 和 Cookie。
7. 用 Wireshark 抓 HTTP 明文，再抓 HTTPS TLS 握手。
8. 在嵌入式设备上实现 HTTP GET/POST。
9. 再实现 HTTPS，处理证书、时间、内存和超时问题。

## 参考资料

- [RFC 9110: HTTP Semantics](https://www.rfc-editor.org/rfc/rfc9110)
- [RFC 9112: HTTP/1.1](https://www.rfc-editor.org/rfc/rfc9112)
- [RFC 9113: HTTP/2](https://www.rfc-editor.org/rfc/rfc9113)
- [RFC 9114: HTTP/3](https://www.rfc-editor.org/rfc/rfc9114)
- [RFC 8446: The Transport Layer Security (TLS) Protocol Version 1.3](https://www.rfc-editor.org/rfc/rfc8446)
- [MDN Web Docs: HTTP](https://developer.mozilla.org/en-US/docs/Web/HTTP)
- [MDN Web Docs: HTTP response status codes](https://developer.mozilla.org/en-US/docs/Web/HTTP/Reference/Status)
- [MDN Web Docs: HTTP cookies](https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/Cookies)
- [curl Documentation](https://curl.se/docs/)
