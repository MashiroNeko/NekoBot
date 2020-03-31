#include <iostream>
#include <set>
#include <sstream>

#include <cqcppsdk/cqcppsdk.h>

using namespace cq;
using namespace std;
using Message = cq::message::Message;
using MessageSegment = cq::message::MessageSegment;

CQ_INIT {
    on_enable([] { logging::info("启用", "插件已启用"); });

    on_private_message([](const PrivateMessageEvent &event) {
        try {
            auto msgid = send_private_message(event.user_id, "NekoBot Activated!"); 
            logging::info_success("私聊", "private promotion succ, messageID: " + to_string(msgid));
            send_message(event.target,
                        "repo地址：https://github.com/MashiroNeko/NekoBot");
            send_message(event.target,"欢迎提交issue呢~");
        } catch (ApiError &err) {
            logging::warning("私聊", "私聊消息复读失败, 错误码: " + to_string(err.code));
        }
    });

    on_message([](const MessageEvent &event) {
        logging::debug("消息", "收到消息: " + event.message + "\n实际类型: " + typeid(event).name());
    });

    on_group_message([](const GroupMessageEvent &event) {
        static const set<int64_t> ENABLED_GROUPS = { 
            417556013,
            736547258
        }; // add QQgroup here
        if (ENABLED_GROUPS.count(event.group_id) == 0) return; // check ENABLED_GROUP

        try {
            if (event.message == "/mars") { send_message(event.target, "建议实时关注本群呢~"); }
            if (event.message == "/nekobot"){ 
                // add future functions here
                send_message(event.target, "NekoBot functions:\n'/mars'"); 
                }
        } catch (ApiError &) { // 忽略发送失败
        }
        event.block(); // 阻止当前事件传递到下一个插件
    });
}

CQ_MENU(menu_demo_1) {
    logging::info("菜单", "点击菜单1");
}

CQ_MENU(menu_demo_2) {
    send_private_message(10000, "测试");
}
