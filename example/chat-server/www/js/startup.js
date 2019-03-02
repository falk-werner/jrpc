/*
 * JRPC - Yet another JSON-RPC server based on libwebsockets
 *  <https://github.com/falk-werner/jrpc>
 *
 * Copyright (c) 2019 Falk Werner
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

let ws = null;
const MESSAGE_ID_SET_NAME = 42;

function logError(message) {    
    const error = document.createElement("div");
    error.className = "error";
    error.textContent = message;

    const chatWindow = document.getElementById("content");
    chatWindow.insertBefore(error, chatWindow.firstChild);
}

function chat(nickname, message) {
    const post = document.createElement("div");
    post.className = "message";

    const nameHolder = document.createElement("span");
    nameHolder.className = "nickname";
    nameHolder.textContent = nickname;
    post.appendChild(nameHolder); 

    const content = document.createElement("span");
    content.className = "content";
    content.textContent = message;
    post.appendChild(content);

    const chatWindow = document.getElementById("content");
    chatWindow.insertBefore(post, chatWindow.firstChild);
}

function announce(what, who) {
    let message = "";
    switch (what) {
        case "arrived":
        message = `${who} has entered`;
        break;
        case "gone":
        message = `${who} left the chat`;
        break;
        default:
        message = `${who} has ${what}`;
        break;
    }

    const announcement = document.createElement("div");
    announcement.className = "announcement";
    announcement.textContent = message;

    const chatWindow = document.getElementById("content");
    chatWindow.insertBefore(announcement, chatWindow.firstChild);
}

function onerror(value)
{
    logError(value);
}

function onopen() {
    const nickname = document.getElementById("nickname").value;
    ws.send(JSON.stringify({
        method: "set_name",
        params: [nickname],
        id: MESSAGE_ID_SET_NAME
    }));
}

function processNotification(method, params) {
    if ("message" === method) {
        const [name, content] = params;
        chat(name, content);
    }
    else if ("announce" === method) {
        const [what, who] = params;
        announce(what, who);
    }
}

function processResponse(response) {
    if (MESSAGE_ID_SET_NAME === response.id) {
        if (response.error) {
            logError(response.error.message || "failed to set nickname");

            const temp = ws;
            ws = null;
            temp.close();
        }    
    }
}

function onmessage(message) {
    try {
        const parsed = JSON.parse(message.data);
        if (typeof(parsed.method) === "string") {
            processNotification(parsed.method, parsed.params);
        }
        else if (typeof(parsed.id) === "number") {
            processResponse(parsed);
        }
    }
    catch (ex) {
        // swallow
    }
}

function connect() {
    const server = document.getElementById("server").value;

    if (null != ws) {
        ws.close();
    }

    ws = new WebSocket(server, "jrpc-chat");
    ws.onerror = onerror;
    ws.onopen = onopen;
    ws.onmessage = onmessage;
}

function submit() {
    const message = document.getElementById("message");
    ws.send(JSON.stringify({
        method: "chat",
        params: [message.value]        
    }));

    message.value = "";
}

function startup()
{
    const connectButton = document.getElementById("connect_button");
    connectButton.addEventListener("click", connect);

    const submitButton = document.getElementById("submit_button");
    submitButton.addEventListener("click", submit);

    const messageTextbox = document.getElementById("message");
    messageTextbox.addEventListener("keyup", (event) => {
        if (event.keyCode === 13) {
            event.preventDefault();
            submitButton.click();
        }
    });
}

 window.onload = startup();