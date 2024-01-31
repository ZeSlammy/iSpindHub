#include "templatescreen.h"
extern TFT_eSPI tft;
extern String SG;
extern String Voltage;
extern String Temp;
extern String Angle;
extern String deviceName;
extern String RSSI;
extern String IP;
extern String LastSeen;
void parse_screen_template(String screen_template, String array_data[10], int last_seen_ms)
{
    wdt_disable();
    // Set Up the Scene with variables
    SG = array_data[5];
    Voltage = array_data[4].substring(0, 4);
    Temp = array_data[3];
    Angle = array_data[2].substring(0, 5);
    deviceName = array_data[1];
    RSSI = array_data[7];
    IP = WiFi.localIP().toString();
    LastSeen = String(pretty_time(last_seen_ms));

    // Open the Screen Template
    String fname = String("/data/templates/") + String(screen_template) + String(".json");

    File file = LittleFS.open(fname, "r");
    String parsingScreen = file.readString();
    JsonDocument parsedScreen;
    // ReadLoggingStream loggingStream(parsingScreen, Serial);
    // ReadLoggingStream loggingStream(file, Serial);
    // DeserializationError errPars = deserializeJson(parsedScreen, loggingStream);
    // DeserializationError errPars = deserializeJson(parsedScreen,file);
    DeserializationError errPars = deserializeJson(parsedScreen, parsingScreen);
    if (errPars)
    {
        Serial.print(F("deserializeJson() for template failed: "));
        Serial.println(errPars.f_str());
        return;
    }
    else
    {
        JsonObject ScreenTemp = parsedScreen.as<JsonObject>();
        for (JsonPair p : ScreenTemp)
        {
            Serial.println(p.key().c_str()); // is a JsonString
            // Serial.println(p.value().as<String>()); // is a JsonVariant
            String key_json = p.key().c_str();
            if (key_json == "g")
            {
                // Serial.println("Let's handle Global parameters");
                handle_global(p.value().as<String>());
            }
            else if (key_json.substring(0, 4) == "line")
            {
                // Serial.println("Let's hanndle parameters for a line");
                handle_line(p.value().as<String>());
            }
            else
            {
                Serial.println("Entry in template not recognized");
            }
        }
    }
    wdt_enable(WDTO_8S);
    parsedScreen.clear();
    return;
};

void handle_global(String global_json)
{
    JsonDocument parsedGlobal;
    DeserializationError errPars = deserializeJson(parsedGlobal, global_json);
    if (errPars)
    {
        Serial.print(F("deserializeJson() for Global failed: "));
        Serial.println(errPars.f_str());
        return;
    }
    else
    {
        JsonObject globalTemp = parsedGlobal.as<JsonObject>();
        for (JsonPair p : globalTemp)
        {
            String key_json = p.key().c_str();
            if (key_json == "bc")
            {
                String my_col = p.value().as<String>();
                uint32_t back_col = get_color(my_col);
                // Serial.println("We found a back color");
                tft.fillScreen(back_col);
            }
            else if (key_json == "fc")
            {
                String my_col = p.value().as<String>();
                uint32_t font_col = get_color(my_col);
                // Serial.println("We found a font color");
                tft.setTextColor(font_col);
            }
            else if (key_json == "tw")
            {
                bool wrap_flag = p.value().as<bool>();
                // Serial.println("We found a Wrap Flag");
                tft.setTextWrap(wrap_flag);
            }
            else if (key_json == "r")
            {
                int screen_rotation = p.value().as<int>();
                // Serial.println("We found a rotation flag");
                tft.setRotation(screen_rotation);
            }
            else if (key_json == "f")
            {
                // uint8_t default_font = p.value().as<uint8_t>();
                // String def_font = p.value().as<String()>;
                // Serial.println("We found default Font");
                if (p.value())
                {
                    tft.loadFont(p.value(), LittleFS);
                }

                // tft.setTextFont(default_font);
            }
        }
    }
    parsedGlobal.clear();
};

void handle_line(String line_json)
{
    JsonDocument parsedLine;
    DeserializationError errPars = deserializeJson(parsedLine, line_json);
    if (errPars)
    {
        Serial.print(F("deserializeJson() for Line failed: "));
        Serial.println(errPars.f_str());
        return;
    }
    else
    {
        JsonObject lineTemp = parsedLine.as<JsonObject>();
        if (lineTemp.containsKey("t"))
        {
            if (lineTemp["t"] == "text")
            {
                // Serial.println("Let's deal with a text line");
                make_text_line(lineTemp);
            }
            else if (lineTemp["t"] == "var_text")
            {
                Serial.println("Let's deal with a VARIABLE text line");
            }
            else if (lineTemp["t"] == "line")
            {
                Serial.println("Let's deal with a line line");
                make_line_line(lineTemp);
            }
        }
    }
    // parsedLine.clear();
    return;
}

void make_text_line(JsonObject text_line_json)
{
    // {"type":"text","font":"","color":"","x":0,"y":18,"center":false,"text":"SG: #SG"}
    String line_text = text_line_json["text"];
    line_text.replace("#SG", SG);
    line_text.replace("#Temp", Temp);
    line_text.replace("#Angle", Angle);
    line_text.replace("#RSSI", RSSI);
    line_text.replace("#Voltage", Voltage);
    line_text.replace("#IP", IP);
    line_text.replace("#LastSeen", LastSeen);
    line_text.replace("#deviceName", deviceName);
    bool center_flag = text_line_json["ctr"];
    int x_pos;
    int y_pos;
    if (text_line_json["x"] == "MAX")
    {
        x_pos = TFT_WIDTH;
    }
    else
    {
        x_pos = text_line_json["x"];
    }
    if (text_line_json["y"] == "MAX")
    {
        y_pos = TFT_HEIGHT;
    }
    else
    {
        y_pos = text_line_json["y"];
    }
    if (text_line_json.containsKey("x_delta"))
    {
        x_pos = x_pos + text_line_json["x_delta"].as<float>();
    }
    if (text_line_json.containsKey("y_delta"))
    {
        y_pos = y_pos + text_line_json["y_delta"].as<float>();
    }
    String text_font = text_line_json["f"];
    String text_color = text_line_json["c"];
    if (text_font.length() > 0)
    {
        char *da_font = get_font(text_font);
        tft.loadFont(da_font, LittleFS);
    }
    if (text_line_json.containsKey("s"))
    {
        int text_size = text_line_json["s"];
        tft.setFreeFont();
        tft.setTextSize(text_size);
    }

    if (text_line_json.containsKey("var"))
    {
        if (text_line_json.containsKey("cs"))
        {
            String tmp_var = text_line_json["var"];
            int val_check;
            if (tmp_var == "#SG")
            {
                val_check = SG.toInt();
            }
            else if ((tmp_var == "#RSSI"))
            {
                val_check = RSSI.substring(1, 3).toInt();
            }
            else if ((tmp_var == "#Voltage"))
            {
                val_check = Voltage.toInt();
            }
            else if ((tmp_var == "#Temp"))
            {
                val_check = Temp.toInt();
            }
            else if ((tmp_var == "#Angle"))
            {
                val_check = Angle.toInt();
            }

            int val_low = text_line_json["cs"][0]["val"].as<int>();
            String col_low = text_line_json["cs"][0]["col"];
            int val_medium = text_line_json["cs"][1]["val"].as<int>();
            String col_medium = text_line_json["cs"][1]["col"];
            int val_high = text_line_json["cs"][2]["val"].as<int>();
            String col_high = text_line_json["cs"][2]["col"];
            String col_def = text_line_json["def_col"];
            /*
            Serial.println(val_check);
            Serial.println(val_low);
            Serial.println(val_medium);
            Serial.println(val_high);
            */
            if (val_check < val_low)
            {
                text_color = col_low;
            }
            else if (val_check < val_medium)
            {
                text_color = col_medium;
            }
            else if (val_check < val_high)
            {
                text_color = col_high;
            }
            else
            {
                text_color = col_def;
            }
            // Serial.println(text_color);
            uint16_t t_col = get_color(text_color);
            tft.setTextColor(t_col);
        }
        else
        {
            if (text_color.length() > 0)
            {
                // Serial.println("Let's change the Color for this line");
                uint16_t t_col = get_color(text_color);
                tft.setTextColor(t_col);
            }
        }
    }
    else
    {
        if (text_color.length() > 0)
        {
            // Serial.println("Let's change the Color for this line");
            uint16_t t_col = get_color(text_color);
            tft.setTextColor(t_col);
        }
    }
    if (center_flag)
    {
        // Serial.println("Printing a Centered Line");
        centerString(line_text, (TFT_WIDTH / 2), y_pos);
    }
    else
    {
        // Serial.println("Printing a regular line");
        tft.setCursor(x_pos, y_pos);
        tft.print(line_text);
    }
    // Serial.println(line_text);
    tft.unloadFont();
    return;
}

void make_line_line(JsonObject line_line_json)
{
    // Serial.println(line_line_json);
    int x_0 = line_line_json["x_0"];
    int y_0 = line_line_json["y_0"];
    int x_1;
    if (line_line_json["x_1"] == "MAX")
    {
        x_1 = TFT_WIDTH;
    }
    else
    {
        x_1 = line_line_json["x_1"];
    }
    int y_1;
    if (line_line_json["y_1"] == "MAX")
    {
        y_1 = TFT_HEIGHT;
    }
    else
    {
        y_1 = line_line_json["y_1"];
    }
    String line_color = line_line_json["c"];
    uint16_t l_col = get_color(line_color);
    tft.drawLine(x_0, y_0, x_1, y_1, l_col);
}

char *get_font(String font)
{
    if (font == "FreeSansGras12")
    {
        return FreeSansGras12;
    }
    else if (font == "FreeSans9")
    {
        return FreeSans9;
    }
    else if (font == "Arial9")
    {
        return Arial9;
    }
    else if (font == "Arial12")
    {
        return Arial12;
    }
    else if (font == "Arial20")
    {
        return Arial20;
    }
    else if (font == "RThin9")
    {
        return RThin9;
    }
    else if (font == "RThin12")
    {
        return RThin12;
    }
    else if (font == "RThin20")
    {
        return RThin20;
    }
    else if (font == "RBold20")
    {
        return RBold20;
    }
    else if (font == "TMS10")
    {
        return TMS10;
    }
    else if (font == "TMS12")
    {
        return TMS12;
    }
    else if (font == "SegLight20")
    {
        return SegLight20;
    }
    return RThin20;
}
