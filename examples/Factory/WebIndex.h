/**
 * @file      WebIndex.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-01-05
 * @note      Just for testing, no functionality
 */

#pragma once

const char *web_html = R"EOF(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Simple Mobile Settings Page</title>
    
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f4f4f4;
            margin: 0;
            padding: 0;
        }
        .container {
            max-width: 600px;
            margin: 0 auto;
            padding: 20px;
        }
        .setting-item {
            background-color: #fff;
            margin-bottom: 10px;
            padding: 20px;
            border-radius: 5px;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
        }
        .setting-item label {
            display: block;
        }
        .toggle-switch {
            display: inline-block;
            width: 60px;
            height: 34px;
            position: relative;
            float: right;
        }
        .toggle-switch input {
            display: none;
        }
        .slider {
            position: absolute;
            cursor: pointer;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: #ccc;
            transition: 0.4s;
            border-radius: 34px;
        }
        .slider:before {
            position: absolute;
            content: "";
            height: 26px;
            width: 26px;
            left: 4px;
            bottom: 4px;
            background-color: white;
            transition: 0.4s;
            border-radius: 50%;
        }
        input:checked + .slider {
            background-color: #2196F3;
        }
        input:checked + .slider:before {
            transform: translateX(26px);
        }
        .setting-item input[type="text"],
        .setting-item input[type="password"] {
            float: right;
        }
        .setting-item::after {
            content: "";
            clear: both;
            display: table;
        }
    </style>
</head>
<body>
    <div class="container">
    <h2 style="text-align: left; color: #333;">TWR Setting</h2>

        <div class="setting-item">
            <label for="setting1">Setting 1</label>
            <div class="toggle-switch" onclick="toggleSwitch(this.firstChild.id)">
                <input type="checkbox" id="setting1">
                <span class="slider"></span>
            </div>
        </div>
        <div class="setting-item">
            <label for="setting2">Setting 2</label>
            <div class="toggle-switch" onclick="toggleSwitch(this.firstChild.id)">
                <input type="checkbox" id="setting2">
                <span class="slider"></span>
            </div>
        </div>
        
        <div class="setting-item">
            <label for="wifiUsername">Wi-Fi Username</label>
            <input type="text" id="wifiUsername" name="wifiUsername" placeholder="Enter Wi-Fi username">
        </div>
        <div class="setting-item">
            <label for="wifiPassword">Wi-Fi Password</label>
            <input type="password" id="wifiPassword" name="wifiPassword" placeholder="Enter Wi-Fi password">
        </div>
        
        <button style="width: 100%; padding: 15px; border: none; border-radius: 5px; background-color: #2196F3; color: white; font-size: 16px; cursor: pointer;" onclick="submitSettings()">Submit Settings</button>
<!-- Add more settings here -->

    </div>

    <script>
        function submitSettings() {
            // Placeholder function for submitting settings
            // Here you can add code to handle the submission of settings
            alert("Settings submitted!");
        }
    </script>

    <script>
        function toggleSwitch(switchId) {
            var checkbox = document.getElementById(switchId);
            // Manually toggling the checkbox state
            checkbox.checked = !checkbox.checked;
        }

        document.addEventListener('DOMContentLoaded', function() {
            // Adding click event listeners to all toggle-switch divs
            var switches = document.querySelectorAll('.toggle-switch');
            switches.forEach(function(switchElement) {
                switchElement.addEventListener('click', function() {
                    toggleSwitch(this.children[0].id);
                });
            });
        });

        function submitSettings() {
            // Placeholder function for submitting settings
            // Here you can add code to handle the submission of settings
            alert("Settings submitted!");
        }
    </script>
</body>
</html>
)EOF";