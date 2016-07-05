#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>

#include <profapi.h>

static PROF_WIN_TAG plugin_win = "C Test";
static pthread_t worker_thread;
static int count = 0;
static int ping_id = 1;

void*
inc_counter(void *arg)
{
    while (1) {
        sleep(5);
        count++;
    }
}

void
handle_win_input(PROF_WIN_TAG win, char *line)
{
    char *str = "Input received: ";
    char buf[strlen(str) + strlen(line) + 1];
    sprintf(buf, "%s%s", str, line);
    prof_win_show(win, buf);
}

void
create_win(void)
{
    if (!prof_win_exists(plugin_win)) {
        prof_win_create(plugin_win, handle_win_input);
    }
}

void
consalert(void)
{
    create_win();
    prof_win_focus(plugin_win);
    prof_cons_alert();
    prof_win_show(plugin_win, "called -> prof_cons_alert");
}

void
consshow(char *msg)
{
    if (msg == NULL) {
        prof_cons_bad_cmd_usage("/c-test");
        return;
    }
    create_win();
    prof_win_focus(plugin_win);
    prof_cons_show(msg);
    char *str = "called -> prof_cons_show: ";
    char buf[strlen(str) + strlen(msg)];
    sprintf(buf, "%s%s", str, msg);
    prof_win_show(plugin_win, buf);
}

void
consshow_t(char *group, char *key, char *def, char *msg)
{
    if (group == NULL || key == NULL || def == NULL || msg == NULL) {
        prof_cons_bad_cmd_usage("/c-test");
        return;
    }
    create_win();
    prof_win_focus(plugin_win);
    char *groupval = strcmp(group, "none") == 0 ? NULL : group;
    char *keyval = strcmp(key, "none") == 0 ? NULL : key;
    char *defval = strcmp(def, "none") == 0 ? NULL : def;
    prof_cons_show_themed(groupval, keyval, defval, msg);
    char *str = "called -> prof_cons_show_themed: ";
    char buf[strlen(str) + strlen(group) + 2 + strlen(key) + 2 + strlen(def) + 2 + strlen(msg)];
    sprintf(buf, "%s%s, %s, %s, %s", str, group, key, def, msg);
    prof_win_show(plugin_win, buf);
}

void
constest(void)
{
    int res = prof_current_win_is_console();
    create_win();
    prof_win_focus(plugin_win);
    if (res) {
        prof_win_show(plugin_win, "called -> prof_current_win_is_console: true");
    } else {
        prof_win_show(plugin_win, "called -> prof_current_win_is_console: false");
    }
}

void
winshow(char *msg)
{
    if (msg == NULL) {
        prof_cons_bad_cmd_usage("/c-test");
        return;
    }
    create_win();
    prof_win_focus(plugin_win);
    prof_win_show(plugin_win, msg);
    char *str = "called -> prof_win_show: ";
    char buf[strlen(str) + strlen(msg)];
    sprintf(buf, "%s%s", str, msg);
    prof_win_show(plugin_win, buf);
}

void
winshow_t(char *group, char *key, char *def, char *msg)
{
    if (group == NULL || key == NULL || def == NULL || msg == NULL) {
        prof_cons_bad_cmd_usage("/c-test");
        return;
    }
    create_win();
    prof_win_focus(plugin_win);
    char *groupval = strcmp(group, "none") == 0 ? NULL : group;
    char *keyval = strcmp(key, "none") == 0 ? NULL : key;
    char *defval = strcmp(def, "none") == 0 ? NULL : def;
    prof_win_show_themed(plugin_win, groupval, keyval, defval, msg);
    char *str = "called -> prof_win_show_themed: ";
    char buf[strlen(str) + strlen(group) + 2 + strlen(key) + 2 + strlen(def) + 2 + strlen(msg)];
    sprintf(buf, "%s%s, %s, %s, %s", str, group, key, def, msg);
    prof_win_show(plugin_win, buf);
}

void
sendline(char *line)
{
    if (line == NULL) {
        prof_cons_bad_cmd_usage("/c-test");
        return;
    }
    create_win();
    prof_win_focus(plugin_win);
    prof_send_line(line);
    char *str = "called -> prof_send_line: ";
    char buf[strlen(str) + strlen(line)];
    sprintf(buf, "%s%s", str, line);
    prof_win_show(plugin_win, buf);
}

void
donotify(char *msg)
{
    if (msg == NULL) {
        prof_cons_bad_cmd_usage("/c-test");
        return;
    }
    create_win();
    prof_win_focus(plugin_win);
    prof_notify(msg, 5000, "c-test plugin");
    char *str = "called -> prof_notify: ";
    char buf[strlen(str) + strlen(msg)];
    sprintf(buf, "%s%s", str, msg);
    prof_win_show(plugin_win, buf);
}

void
getsubject(char *subject)
{
    if (subject == NULL) {
        prof_cons_bad_cmd_usage("/c-test");
        return;
    }

    if (strcmp(subject, "recipient") == 0) {
        create_win();
        char *recipient = prof_get_current_recipient();
        if (recipient) {
            prof_win_focus(plugin_win);
            char *str = "called -> prof_get_current_recipient: ";
            char buf[strlen(str) + strlen(recipient)];
            sprintf(buf, "%s%s", str, recipient);
            prof_win_show(plugin_win, buf);
        } else {
            prof_win_focus(plugin_win);
            prof_win_show(plugin_win, "called -> prof_get_current_recipient: <none>");
        }
    } else if (strcmp(subject, "room") == 0) {
        create_win();
        char *room = prof_get_current_muc();
        if (room) {
            prof_win_focus(plugin_win);
            char *str = "called -> prof_get_current_muc: ";
            char buf[strlen(str) + strlen(room)];
            sprintf(buf, "%s%s", str, room);
            prof_win_show(plugin_win, buf);
        } else {
            prof_win_focus(plugin_win);
            prof_win_show(plugin_win, "called -> prof_get_current_muc: <none>");
        }
    } else if (strcmp(subject, "nick") == 0) {
        create_win();
        char *nick = prof_get_current_nick();
        if (nick) {
            prof_win_focus(plugin_win);
            char *str = "called -> prof_get_current_nick: ";
            char buf[strlen(str) + strlen(nick)];
            sprintf(buf, "%s%s", str, nick);
            prof_win_show(plugin_win, buf);
        } else {
            prof_win_focus(plugin_win);
            prof_win_show(plugin_win, "called -> prof_get_current_nick: <none>");
        }
    } else if (strcmp(subject, "occupants") == 0) {
        create_win();
        char **occupants = prof_get_current_occupants();
        if (occupants) {
            prof_win_focus(plugin_win);
            prof_win_show(plugin_win, "called -> prof_get_current_occupants:");
            int i = 0;
            while(occupants[i] != NULL) {
                prof_win_show(plugin_win, occupants[i]);
                i++;
            }
        } else {
            prof_win_focus(plugin_win);
            prof_win_show(plugin_win, "called -> prof_get_current_occupants: <none>");
        }
    } else {
        prof_cons_bad_cmd_usage("/c-test");
    }
}

void
logmsg(char *level, char *msg)
{
    if (level == NULL || msg == NULL) {
        prof_cons_bad_cmd_usage("/c-test");
        return;
    }

    if (strcmp(level, "debug") == 0) {
        create_win();
        prof_win_focus(plugin_win);
        prof_log_debug(msg);
        char *str = "called -> prof_log_debug: ";
        char buf[strlen(str) + strlen(msg)];
        sprintf(buf, "%s%s", str, msg);
        prof_win_show(plugin_win, buf);
    } else if (strcmp(level, "info") == 0) {
        create_win();
        prof_win_focus(plugin_win);
        prof_log_info(msg);
        char *str = "called -> prof_log_info: ";
        char buf[strlen(str) + strlen(msg)];
        sprintf(buf, "%s%s", str, msg);
        prof_win_show(plugin_win, buf);
    } else if (strcmp(level, "warning") == 0) {
        create_win();
        prof_win_focus(plugin_win);
        prof_log_warning(msg);
        char *str = "called -> prof_log_warning: ";
        char buf[strlen(str) + strlen(msg)];
        sprintf(buf, "%s%s", str, msg);
        prof_win_show(plugin_win, buf);
    } else if (strcmp(level, "error") == 0) {
        create_win();
        prof_win_focus(plugin_win);
        prof_log_error(msg);
        char *str = "called -> prof_log_error: ";
        char buf[strlen(str) + strlen(msg)];
        sprintf(buf, "%s%s", str, msg);
        prof_win_show(plugin_win, buf);
    } else {
        prof_cons_bad_cmd_usage("/c-test");
    }
}

void
docount(void)
{
    create_win();
    prof_win_focus(plugin_win);
    char buf[100];
    sprintf(buf, "Count: %d", count);
    prof_win_show(plugin_win, buf);
}

void
doping(char *jid)
{
    if (jid == NULL) {
        prof_cons_bad_cmd_usage("/c-test");
        return;
    }
    create_win();
    prof_win_focus(plugin_win);
    char *strstart = "<iq to='";
    char *strend = "' type='get'><ping xmlns='urn:xmpp:ping'/></iq>";
    char *idstart = "' id='cplugin-";
    char idbuf[strlen(idstart) + 10];
    sprintf(idbuf, "%s%d", idstart, ping_id);
    char buf[strlen(strstart) + strlen(jid) + strlen(idbuf) + strlen(strend)];
    sprintf(buf, "%s%s%s%s", strstart, jid, idbuf, strend);
    int res = prof_send_stanza(buf);
    ping_id++;
    if (res) {
        prof_win_show(plugin_win, "Ping sent successfully");
    } else {
        prof_win_show(plugin_win, "Error sending ping");
    }
}

void
booleansetting(char *op, char *group, char *key, char *value_str)
{
    if (op == NULL || group == NULL || key == NULL) {
        prof_cons_bad_cmd_usage("/c-test");
        return;
    }

    if ((strcmp(op, "get") != 0) && (strcmp(op, "set") != 0)) {
        prof_cons_bad_cmd_usage("/c-test");
        return;
    }

    if ((strcmp(op, "set") == 0) && (strcmp(value_str, "true") != 0) && (strcmp(value_str, "false") != 0)) {
        prof_cons_bad_cmd_usage("/c-test");
        return;
    }

    if (strcmp(op, "get") == 0) {
        int dflt = 0;
        create_win();
        prof_win_focus(plugin_win);
        int res = prof_settings_get_boolean(group, key, dflt);
        if (res) {
            prof_win_show(plugin_win, "Boolean setting: TRUE");
        } else {
            prof_win_show(plugin_win, "Boolean setting: FALSE");
        }
    } else if (strcmp(op, "set") == 0) {
        int value = 0;
        if (strcmp(value_str, "true") == 0) {
            value = 1;
        }
        create_win();
        prof_win_focus(plugin_win);
        prof_settings_set_boolean(group, key, value);
        char buf[5 + strlen(group) + 2 + strlen(key) + 4 + strlen(value_str)];
        sprintf(buf, "Set [%s] %s to %s", group, key, value_str);
        prof_win_show(plugin_win, buf);
    }
}

void
stringsetting(char *op, char *group, char *key, char *value)
{
    if (op == NULL || group == NULL || key == NULL) {
        prof_cons_bad_cmd_usage("/c-test");
        return;
    }

    if ((strcmp(op, "get") != 0) && (strcmp(op, "set") != 0)) {
        prof_cons_bad_cmd_usage("/c-test");
        return;
    }

    if ((strcmp(op, "set") == 0) && (value == NULL)) {
        prof_cons_bad_cmd_usage("/c-test");
        return;
    }

    if (strcmp(op, "get") == 0) {
        create_win();
        prof_win_focus(plugin_win);
        char *res = prof_settings_get_string(group, key, NULL);
        if (res) {
            char buf[16 + strlen(res)];
            sprintf(buf, "String setting: %s", res);
            prof_win_show(plugin_win, buf);
        } else {
            prof_win_show(plugin_win, "String setting: NULL");
        }
    } else if (strcmp(op, "set") == 0) {
        create_win();
        prof_win_focus(plugin_win);
        prof_settings_set_string(group, key, value);
        char buf[5 + strlen(group) + 2 + strlen(key) + 4 + strlen(value)];
        sprintf(buf, "Set [%s] %s to %s", group, key, value);
        prof_win_show(plugin_win, buf);
    }
}

void
intsetting(char *op, char *group, char *key, char *value)
{
    if (op == NULL || group == NULL || key == NULL) {
        prof_cons_bad_cmd_usage("/c-test");
        return;
    }

    if ((strcmp(op, "get") != 0) && (strcmp(op, "set") != 0)) {
        prof_cons_bad_cmd_usage("/c-test");
        return;
    }

    if (strcmp(op, "get") == 0) {
        create_win();
        prof_win_focus(plugin_win);
        int res = prof_settings_get_int(group, key, 0);
        char buf[256];
        sprintf(buf, "Integer setting: %d", res);
        prof_win_show(plugin_win, buf);
    } else if (strcmp(op, "set") == 0) {
        create_win();
        prof_win_focus(plugin_win);
        int int_value = atoi(value);
        prof_settings_set_int(group, key, int_value);
        char buf[256];
        sprintf(buf, "Set [%s] %s to %d", group, key, int_value);
        prof_win_show(plugin_win, buf);
    }
}

void
incomingmsg(char *barejid, char *resource, char *message)
{
    prof_incoming_message(barejid, resource, message);
}

void
completer(char *op, char *item)
{
    if (item == NULL) {
        prof_cons_bad_cmd_usage("/c-test");
        return;        
    }

    if (strcmp(op, "add") == 0) {
        create_win();
        prof_win_focus(plugin_win);
        char *ac[] = { item, NULL };
        prof_completer_add("/c-test", ac);
        char buf[256];
        sprintf(buf, "Added \"%s\" to /c-test completer", item);
        prof_win_show(plugin_win, buf);
        prof_completer_add("/c-test completer remove", ac);
    } else if (strcmp(op, "remove") == 0) {
        create_win();
        prof_win_focus(plugin_win);
        char *ac[] = { item, NULL };
        prof_completer_remove("/c-test", ac);
        char buf[256];
        sprintf(buf, "Removed \"%s\" from /c-test completer", item);
        prof_win_show(plugin_win, buf);
        prof_completer_remove("/c-test completer remove", ac);
    } else {
        prof_cons_bad_cmd_usage("/c-test");
    }
}

void
cmd_ctest(char **args)
{
    if      (strcmp(args[0], "consalert") == 0)     consalert();
    else if (strcmp(args[0], "consshow") == 0)      consshow(args[1]);
    else if (strcmp(args[0], "consshow_t") == 0)    consshow_t(args[1], args[2], args[3], args[4]);
    else if (strcmp(args[0], "constest") == 0)      constest();
    else if (strcmp(args[0], "winshow") == 0)       winshow(args[1]);
    else if (strcmp(args[0], "winshow_t") == 0)     winshow_t(args[1], args[2], args[3], args[4]);
    else if (strcmp(args[0], "sendline") == 0)      sendline(args[1]);
    else if (strcmp(args[0], "notify") == 0)        donotify(args[1]);
    else if (strcmp(args[0], "get") == 0)           getsubject(args[1]);
    else if (strcmp(args[0], "log") == 0)           logmsg(args[1], args[2]);
    else if (strcmp(args[0], "count") == 0)         docount();
    else if (strcmp(args[0], "ping") == 0)          doping(args[1]);
    else if (strcmp(args[0], "boolean") == 0)       booleansetting(args[1], args[2], args[3], args[4]);
    else if (strcmp(args[0], "string") == 0)        stringsetting(args[1], args[2], args[3], args[4]);
    else if (strcmp(args[0], "int") == 0)           intsetting(args[1], args[2], args[3], args[4]);
    else if (strcmp(args[0], "incoming") == 0)      incomingmsg(args[1], args[2], args[3]);
    else if (strcmp(args[0], "completer") == 0)     completer(args[1], args[2]);
    else                                            prof_cons_bad_cmd_usage("/c-test");
}

void
timed_callback(void)
{
    create_win();
    prof_win_show(plugin_win, "timed -> timed_callback called");
}

void
prof_init(const char * const version, const char * const status, const char *const account_name, const char *const fulljid)
{
    pthread_create(&worker_thread, NULL, inc_counter, NULL);

    prof_disco_add_feature("urn:xmpp:profanity:c_test_plugin");

    prof_win_create(plugin_win, handle_win_input);

    char buf[256];
    if (account_name != NULL && fulljid != NULL) {
        sprintf(buf, "fired -> prof_init: %s, %s, %s, %s", version, status, account_name, fulljid);
    } else {
        sprintf(buf, "fired -> prof_init: %s, %s", version, status);
    }
    prof_win_show(plugin_win, buf);

    const char *synopsis[] = {
        "/c-test consalert",
        "/c-test consshow <message>",
        "/c-test consshow_t <group> <key> <default> <message>",
        "/c-test constest",
        "/c-test winshow <message>",
        "/c-test winshow_t <group> <key> <default> <message>",
        "/c-test notify <message>",
        "/c-test sendline <line>",
        "/c-test get recipient|room|nick|occupants",
        "/c-test log debug|info|warning|error <message>",
        "/c-test count",
        "/c-test ping <jid>",
        "/c-test boolean get <group> <key>",
        "/c-test boolean set <group> <key> <value>",
        "/c-test string get <group> <key>",
        "/c-test string set <group> <key> <value>",
        "/c-test int get <group> <key>",
        "/c-test int set <group> <key> <value>",
        "/c-test incoming <barejid> <resource> <message>",
        "/c-test completer add|remove <item>",
        NULL
    };
    const char *description = "C test plugin. All commands focus the plugin window.";
    const char *args[][2] = {
        { "consalert",                                      "Highlight the console window in the status bar" },
        { "consshow <message>",                             "Show the message in the console window" },
        { "consshow_t <group> <key> <default> <message>",   "Show the themed message in the console window. " },
        { "constest",                                       "Show whether the command was run in the console." },
        { "winshow <message>",                              "Show the message in the plugin window" },
        { "winshow_t <group> <key> <default> <message>",    "Show the themed message in the plugin window. " },
        { "notify <message>",                               "Send a desktop notification with message" },
        { "sendline <line>",                                "Pass line to profanity to process" },
        { "get recipient",                                  "Show the current chat recipient, if in a chat window" },
        { "get room",                                       "Show the current room JID, if in a chat room" },
        { "get nick",                                       "Show nickname in current room, if in a chat room" },
        { "get occupants",                                  "Show occupants in current room, if in a chat room" },
        { "log debug|info|warning|error <message>",         "Log a message at the specified level" },
        { "count",                                          "Show the counter, incremented every 5 seconds by a worker thread" },
        { "ping <jid>",                                     "Send an XMPP ping to the specified Jabber ID" },
        { "boolean get <group> <key>",                      "Get a boolean setting" },
        { "boolean set <group> <key> <value>",              "Set a boolean setting" },
        { "string get <group> <key>",                       "Get a string setting" },
        { "string set <group> <key> <value>",               "Set a string setting" },
        { "int get <group> <key>",                          "Get a integer setting" },
        { "int set <group> <key> <value>",                  "Set a integer setting" },
        { "incoming <barejid> <resource> <message>",        "Show an incoming message." },
        { "completer add <item>",                           "Add an autocomplete item to the /c-test command." },
        { "completer remove <item>",                        "Remove an autocomplete item from the /c-test command." },
        { NULL, NULL }
    };

    const char *examples[] = {
        "/c-test sendline /about",
        "/c-test log debug \"Test debug message\"",
        "/c-test consshow_t c-test cons.show none \"This is themed\"",
        "/c-test consshow_t none none bold_cyan \"This is bold_cyan\"",
        "/c-test ping buddy@server.org",
        NULL
    };

    prof_register_command("/c-test", 1, 5, synopsis, description, args, examples, cmd_ctest);

    char *cmd_ac[] = {
        "consalert",
        "consshow",
        "consshow_t",
        "constest",
        "winshow",
        "winshow_t",
        "notify",
        "sendline",
        "get",
        "log",
        "count",
        "ping",
        "boolean",
        "string",
        "int",
        "incoming",
        "completer",
        NULL
    };
    prof_completer_add("/c-test", cmd_ac);

    char *get_ac[] = { "recipient", "room", "nick", "occupants", NULL };
    prof_completer_add("/c-test get", get_ac);

    char *log_ac[] = { "debug", "info", "warning", "error", NULL };
    prof_completer_add("/c-test log", log_ac);

    char *boolean_ac[] = { "get", "set", NULL };
    prof_completer_add("/c-test boolean", boolean_ac);

    char *string_ac[] = { "get", "set", NULL };
    prof_completer_add("/c-test string", string_ac);

    char *int_ac[] = { "get", "set", NULL };
    prof_completer_add("/c-test int", int_ac);

    char *completer_ac[] = { "add", "remove", NULL };
    prof_completer_add("/c-test completer", completer_ac);

    prof_register_timed(timed_callback, 30);
}

void
prof_on_start(void)
{
    create_win();
    prof_win_show(plugin_win, "fired -> prof_on_start");
}

void
prof_on_shutdown(void)
{
    create_win();
    prof_win_show(plugin_win, "fired -> prof_on_shutdown");
}

void
prof_on_unload(void)
{
    create_win();
    prof_win_show(plugin_win, "fired -> prof_on_unload");
}

void
prof_on_connect(const char * const account_name, const char * const fulljid)
{
    create_win();

    char *str = "fired -> prof_on_connect: ";
    char buf[strlen(str) + strlen(account_name) + 2 + strlen(fulljid) + 1];
    sprintf(buf, "%s%s, %s", str, account_name, fulljid);
    prof_win_show(plugin_win, buf);
}

void
prof_on_disconnect(const char * const account_name, const char * const fulljid)
{
    create_win();

    char *str = "fired -> prof_on_disconnect: ";
    char buf[strlen(str) + strlen(account_name) + 2 + strlen(fulljid) + 1];
    sprintf(buf, "%s%s, %s", str, account_name, fulljid);
    prof_win_show(plugin_win, buf);
}

char*
prof_pre_chat_message_display(const char * const jid, const char *message)
{
    create_win();

    char *str = "fired -> prof_pre_chat_message_display: ";
    char buf[strlen(str) + strlen(jid) + 2 + strlen(message) + 1];
    sprintf(buf, "%s%s, %s", str, jid, message);
    prof_win_show(plugin_win, buf);

    return NULL;
}

void
prof_post_chat_message_display(const char * const jid, const char *message)
{
    create_win();

    char *str = "fired -> prof_post_chat_message_display: ";
    char buf[strlen(str) + strlen(jid) + 2 + strlen(message) + 1];
    sprintf(buf, "%s%s, %s", str, jid, message);
    prof_win_show(plugin_win, buf);
}

char*
prof_pre_chat_message_send(const char * const jid, const char *message)
{
    create_win();

    char *str = "fired -> prof_pre_chat_message_send: ";
    char buf[strlen(str) + strlen(jid) + 2 + strlen(message) + 1];
    sprintf(buf, "%s%s, %s", str, jid, message);
    prof_win_show(plugin_win, buf);

    return NULL;
}

void
prof_post_chat_message_send(const char * const jid, const char *message)
{
    create_win();

    char *str = "fired -> prof_post_chat_message_send: ";
    char buf[strlen(str) + strlen(jid) + 2 + strlen(message) + 1];
    sprintf(buf, "%s%s, %s", str, jid, message);
    prof_win_show(plugin_win, buf);
}

char*
prof_pre_room_message_display(const char * const room, const char * const nick, const char *message)
{
    create_win();

    char *str = "fired -> prof_pre_room_message_display: ";
    char buf[strlen(str) + strlen(room) + 2 + strlen(nick) + 2 + strlen(message) + 1];
    sprintf(buf, "%s%s, %s, %s", str, room, nick, message);
    prof_win_show(plugin_win, buf);

    return NULL;
}

void
prof_post_room_message_display(const char * const room, const char * const nick, const char *message)
{
    create_win();

    char *str = "fired -> prof_post_room_message_display: ";
    char buf[strlen(str) + strlen(room) + 2 + strlen(nick) + 2 + strlen(message) + 1];
    sprintf(buf, "%s%s, %s, %s", str, room, nick, message);
    prof_win_show(plugin_win, buf);
}

char *
prof_pre_room_message_send(const char * const room, const char *message)
{
    create_win();

    char *str = "fired -> prof_pre_room_message_send: ";
    char buf[strlen(str) + strlen(room) + 2 + strlen(message) + 1];
    sprintf(buf, "%s%s, %s", str, room, message);
    prof_win_show(plugin_win, buf);

    return NULL;
}

void
prof_post_room_message_send(const char * const room, const char *message)
{
    create_win();

    char *str = "fired -> prof_post_room_message_send: ";
    char buf[strlen(str) + strlen(room) + 2 + strlen(message) + 1];
    sprintf(buf, "%s%s, %s", str, room, message);
    prof_win_show(plugin_win, buf);
}

void
prof_on_room_history_message(const char * const room, const char *const nick, const char *const message, const char *const timestamp)
{
    create_win();

    char *str = "fired -> prof_on_room_history_message: ";
    if (timestamp == NULL) {
        char buf[strlen(str) + strlen(room) + 2 + strlen(nick) + 2 + strlen(message) + 1];
        sprintf(buf, "%s%s, %s, %s", str, room, nick, message);
        prof_win_show(plugin_win, buf);
    } else {
        char buf[strlen(str) + strlen(room) + 2 + strlen(nick) + 2 + strlen(message) + 2 + strlen(timestamp) + 1];
        sprintf(buf, "%s%s, %s, %s, %s", str, room, nick, message, timestamp);
        prof_win_show(plugin_win, buf);
    }
}

char *
prof_pre_priv_message_display(const char * const room, const char * const nick, const char *message)
{
    create_win();

    char *str = "fired -> prof_pre_priv_message_display: ";
    char buf[strlen(str) + strlen(room) + 2 + strlen(nick) + 2 + strlen(message) + 1];
    sprintf(buf, "%s%s, %s, %s", str, room, nick, message);
    prof_win_show(plugin_win, buf);

    return NULL;
}

void
prof_post_priv_message_display(const char * const room, const char * const nick, const char *message)
{
    create_win();

    char *str = "fired -> prof_post_priv_message_display: ";
    char buf[strlen(str) + strlen(room) + 2 + strlen(nick) + 2 + strlen(message) + 1];
    sprintf(buf, "%s%s, %s, %s", str, room, nick, message);
    prof_win_show(plugin_win, buf);
}

char *
prof_pre_priv_message_send(const char * const room, const char * const nick, const char *message)
{
    create_win();

    char *str = "fired -> prof_pre_priv_message_send: ";
    char buf[strlen(str) + strlen(room) + 2 + strlen(nick) + 2 + strlen(message) + 1];
    sprintf(buf, "%s%s, %s, %s", str, room, nick, message);
    prof_win_show(plugin_win, buf);

    return NULL;
}

void
prof_post_priv_message_send(const char * const room, const char * const nick, const char *message)
{
    create_win();

    char *str = "fired -> prof_post_priv_message_send: ";
    char buf[strlen(str) + strlen(room) + 2 + strlen(nick) + 2 + strlen(message) + 1];
    sprintf(buf, "%s%s, %s, %s", str, room, nick, message);
    prof_win_show(plugin_win, buf);
}

char*
prof_on_message_stanza_send(const char *const stanza)
{
    create_win();

    char *str = "fired -> prof_on_message_stanza_send: ";
    char buf[strlen(str) + strlen(stanza) + 1];
    sprintf(buf, "%s%s", str, stanza);
    prof_win_show(plugin_win, buf);

    return NULL;
    // char *new_stanza = strdup("<message id='ktx72v49' to='chanleemoon@ejabberd.local' type='chat' xml:lang='en'><body>Art thou not Romeo, and a Montague?</body></message>");
    // return (char *)new_stanza;
}

int
prof_on_message_stanza_receive(const char *const stanza)
{
    create_win();

    char *str = "fired -> prof_on_message_stanza_receive: ";
    char buf[strlen(str) + strlen(stanza) + 1];
    sprintf(buf, "%s%s", str, stanza);
    prof_win_show(plugin_win, buf);

    return 1;
}

char*
prof_on_presence_stanza_send(const char *const stanza)
{
    create_win();

    char *str = "fired -> prof_on_presence_stanza_send: ";
    char buf[strlen(str) + strlen(stanza) + 1];
    sprintf(buf, "%s%s", str, stanza);
    prof_win_show(plugin_win, buf);

    return NULL;
}

int
prof_on_presence_stanza_receive(const char *const stanza)
{
    create_win();

    char *str = "fired -> prof_on_presence_stanza_receive: ";
    char buf[strlen(str) + strlen(stanza) + 1];
    sprintf(buf, "%s%s", str, stanza);
    prof_win_show(plugin_win, buf);

    return 1;
}

char*
prof_on_iq_stanza_send(const char *const stanza)
{
    create_win();

    char *str = "fired -> prof_on_iq_stanza_send: ";
    char buf[strlen(str) + strlen(stanza) + 1];
    sprintf(buf, "%s%s", str, stanza);
    prof_win_show(plugin_win, buf);

    return NULL;
}

int
prof_on_iq_stanza_receive(const char *const stanza)
{
    create_win();

    char *str = "fired -> prof_on_iq_stanza_receive: ";
    char buf[strlen(str) + strlen(stanza) + 1];
    sprintf(buf, "%s%s", str, stanza);
    prof_win_show(plugin_win, buf);

    return 1;
}

void
prof_on_contact_offline(const char *const barejid, const char *const resource, const char *const status)
{
    create_win();

    char *str = "fired -> prof_on_contact_offline: ";
    int status_len = status == NULL ? 0 : strlen(status) + 2;
    char buf[strlen(str) + strlen(barejid) + 1 + strlen(resource) + 1 + status_len + 1];
    if (status) {
        sprintf(buf, "%s%s/%s \"%s\"", str, barejid, resource, status);
    } else {
        sprintf(buf, "%s%s/%s", str, barejid, resource);
    }
    prof_win_show(plugin_win, buf);
}

void
prof_on_contact_presence(const char *const barejid, const char *const resource, const char *const presence, const char *const status, const int priority)
{
    create_win();

    char *str = "fired -> prof_on_contact_presence: ";
    int status_len = status == NULL ? 0 : strlen(status) + 2;
    char buf[strlen(str) + strlen(barejid) + 1 + strlen(resource) + 1 + strlen(presence) + 1 + 10 + status_len + 1];
    if (status) {
        sprintf(buf, "%s%s/%s %s %d \"%s\"", str, barejid, resource, presence, priority, status);
    } else {
        sprintf(buf, "%s%s/%s %s %d", str, barejid, resource, presence, priority);
    }
    prof_win_show(plugin_win, buf);
}

void
prof_on_chat_win_focus(const char *const barejid)
{
    create_win();
    char *str = "fired -> prof_on_chat_win_focus: ";
    char buf[strlen(str) + strlen(barejid) + 1];
    sprintf(buf, "%s%s", str, barejid);
    prof_win_show(plugin_win, buf);
}

void
prof_on_room_win_focus(const char *const roomjid)
{
    create_win();
    char *str = "fired -> prof_on_room_win_focus: ";
    char buf[strlen(str) + strlen(roomjid) + 1];
    sprintf(buf, "%s%s", str, roomjid);
    prof_win_show(plugin_win, buf);
}
