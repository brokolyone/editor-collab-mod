# Editor Collab Relay Server

### Установка на Selectel (без sudo)
1. Подключитесь по SSH.
2. Перейдите в папку проекта: `cd editor-collab-mod/server`
3. Установите зависимости: `npm install`
4. Запустите сервер: `node index.js`

Если нужно, чтобы сервер работал постоянно после закрытия терминала, используйте:
`nohup node index.js > output.log 2>&1 &`

### Настройка клиента
В настройках мода внутри Geometry Dash (Geode -> Editor Collab -> Settings) укажите IP вашего сервера Selectel, например: `ws://123.123.123.123:8080`
