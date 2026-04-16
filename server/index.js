const WebSocket = require('ws');

const port = process.env.PORT || 8080;
const wss = new WebSocket.Server({ port });

const rooms = {};

wss.on('connection', (ws) => {
    ws.on('message', (message) => {
        try {
            const data = JSON.parse(message);
            
            if (data.type === 'host') {
                let code;
                do {
                    code = Math.floor(100000 + Math.random() * 900000).toString();
                } while (rooms[code]);
                
                rooms[code] = { host: ws, clients: [] };
                ws.roomId = code;
                ws.isHost = true;
                ws.send(JSON.stringify({ type: 'host_created', code }));
            } 
            else if (data.type === 'join') {
                const code = data.code;
                if (rooms[code]) {
                    rooms[code].clients.push(ws);
                    ws.roomId = code;
                    ws.isHost = false;
                    ws.send(JSON.stringify({ type: 'joined_success', code }));
                    rooms[code].host.send(JSON.stringify({ type: 'client_joined' }));
                } else {
                    ws.send(JSON.stringify({ type: 'error', message: 'Room not found' }));
                }
            } 
            else if (data.type === 'host_off') {
                const code = ws.roomId;
                if (code && rooms[code]) {
                    rooms[code].clients.forEach(client => {
                        if (client.readyState === WebSocket.OPEN) {
                            client.send(JSON.stringify({ type: 'host_off', levelData: data.levelData }));
                        }
                    });
                    delete rooms[code];
                    ws.roomId = null;
                    ws.isHost = false;
                }
            }
            else if (data.type === 'disconnect_client') {
                 const code = ws.roomId;
                 if (code && rooms[code]) {
                     if (rooms[code].host.readyState === WebSocket.OPEN) {
                         rooms[code].host.send(JSON.stringify({ type: 'client_left' }));
                     }
                     rooms[code].clients = rooms[code].clients.filter(c => c !== ws);
                 }
                 ws.roomId = null;
            }
            else if (data.type === 'cursor') {
                const code = ws.roomId;
                if (code && rooms[code]) {
                    if (ws.isHost) {
                        rooms[code].clients.forEach(client => {
                            if (client.readyState === WebSocket.OPEN) {
                                client.send(JSON.stringify(data));
                            }
                        });
                    } else {
                        if (rooms[code].host.readyState === WebSocket.OPEN) {
                            rooms[code].host.send(JSON.stringify(data));
                        }
                        rooms[code].clients.forEach(client => {
                            if (client !== ws && client.readyState === WebSocket.OPEN) {
                                client.send(JSON.stringify(data));
                            }
                        });
                    }
                }
            }
        } catch (e) {
            console.error('Invalid message', e);
        }
    });

    ws.on('close', () => {
        const code = ws.roomId;
        if (code && rooms[code]) {
            if (ws.isHost) {
                rooms[code].clients.forEach(client => {
                    if (client.readyState === WebSocket.OPEN) {
                        client.send(JSON.stringify({ type: 'host_disconnected' }));
                    }
                });
                delete rooms[code];
            } else {
                rooms[code].clients = rooms[code].clients.filter(c => c !== ws);
                if (rooms[code].host.readyState === WebSocket.OPEN) {
                    rooms[code].host.send(JSON.stringify({ type: 'client_left' }));
                }
            }
        }
    });
});
console.log(`WebSocket Server started on port ${port}`);