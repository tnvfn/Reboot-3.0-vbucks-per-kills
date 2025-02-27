#pragma once

#define CURL_STATICLIB

#include <iostream>
#include <string>
#include <curl/curl.h>

class DiscordWebhook {
public:

    DiscordWebhook(const char* webhook_url)
    {
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, webhook_url);

            // Discord webhooks accept json type messages, so we set the content-type to json data. (or smth i'm not the best)
            curl_slist* headers = curl_slist_append(NULL, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        }
        else {
            std::cerr << "Error: curl_easy_init() returned NULL pointer" << '\n';
        }
    }

    ~DiscordWebhook()
    {
        curl_global_cleanup();
        curl_easy_cleanup(curl);
    }

    bool handleCode(CURLcode res)
    {
        return res == CURLE_OK;
    }

    inline bool send_message(const std::string& message)
    {
        std::string json = "{\"content\": \"" + message + "\"}";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());

        bool success = handleCode(curl_easy_perform(curl));

        return success;
    }

    inline bool send_embedjson(const std::string ajson)
    {
        std::string json = ajson.contains("embeds") ? ajson : "{\"embeds\": " + ajson + "}";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());

        bool success = handleCode(curl_easy_perform(curl));

        return success;
    }

    inline bool send_embed(const std::string& title, const std::string& description, int color = 0)
    {
        std::string json = "{\"embeds\": [{\"title\": \"" + title + "\", \"description\": \"" + description + "\", \"color\": " + std::to_string(color) + "}]}";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());

        bool success = handleCode(curl_easy_perform(curl));

        return success;
    }


    // I don't know honestly do this work. I will push this update and let's see what is going to happen.
    inline bool send_server_up_message()
    {
        std::string json = "{\"embeds\": [{"
            "\"title\": \"Server Status\","
            "\"description\": \"✅ The server is up, ready up to get ingame!\","
            "\"color\": 3066993,"
            "\"timestamp\": \"2024-01-01T00:00:00Z\""
            "}]}";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());

        bool success = handleCode(curl_easy_perform(curl));

        return success;
    }

private:
    CURL* curl;
};

namespace Information
{
    static std::string UptimeWebHook = ("");
}

static DiscordWebhook UptimeWebHook(Information::UptimeWebHook.c_str());
