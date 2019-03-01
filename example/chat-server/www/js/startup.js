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

function onmessage(message) {
    try {
        const parsed = JSON.parse(message.data);
        if ("message" === parsed.method) {
            const [name, content] = parsed.params;
            chat(name, content);
        }
        else if ("announce" == parsed.method) {
            const [what, who] = parsed.params;
            announce(what, who);
        }
        else if (MESSAGE_ID_SET_NAME === parsed.id) {
            if (parsed.error) {
                logError(parsed.error.message || "failed to set nickname");

                const temp = ws;
                ws = null;
                temp.close();
            }
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