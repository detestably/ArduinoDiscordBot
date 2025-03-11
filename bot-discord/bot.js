const Discord = require('discord.js');
const client = new Discord.Client();
const axios = require('axios');

const ARDUINO_IP = 'SEU_IP_DO_ARDUINO'; // Substitua pelo IP do Arduino
const ARDUINO_PORT = 80; // Porta que o Arduino está escutando

client.on('ready', () => {
    console.log(`Bot conectado como ${client.user.tag}`);
});

client.on('message', async message => {
    if (message.author.bot) return; // Ignora mensagens de outros bots

    if (message.content.startsWith('@arduino')) {
        const args = message.content.split(' ');
        const command = args[1];
        const action = args.slice(2).join(' ');

        try {
            let response;
            switch (command) {
                case 'ligar':
                    response = await sendCommandToArduino('ligar', action);
                    break;
                case 'desligar':
                    response = await sendCommandToArduino('desligar', action);
                    break;
                case 'ajustar':
                    response = await sendCommandToArduino('ajustar', action);
                    break;
                case 'status':
                    response = await sendCommandToArduino('status', action);
                    break;
                default:
                    response = 'Comando não reconhecido.';
                    break;
            }
            message.reply(response);
        } catch (error) {
            message.reply('Erro ao enviar comando para o Arduino.');
        }
    }
});

async function sendCommandToArduino(command, action) {
    const url = `http://${ARDUINO_IP}:${ARDUINO_PORT}/comando`;
    const response = await axios.post(url, { command, action });
    return response.data;
}

client.login('SEU_TOKEN_DO_BOT'); // Substitua pelo token do seu bot
