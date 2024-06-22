from telegram import Update, InlineKeyboardButton, InlineKeyboardMarkup, ReplyKeyboardMarkup, KeyboardButton
from telegram.ext import Updater, CommandHandler, CallbackQueryHandler, MessageHandler, Filters, CallbackContext
import requests

# Вставьте сюда свой токен
TOKEN = '7464192291:AAHO2_BO2y_3nhoNrrHC9ewXhvVeInbmfr0'
# Список каналов для подписки
CHANNELS = ['@melodofi']
# Секретный админский код
ADMIN_CODE = 'mane'

# Хранилище балансов пользователей (для упрощения примера)
user_balances = {}

# Команда /start
def start(update: Update, context: CallbackContext) -> None:
    user_id = update.message.from_user.id
    if check_subscription(user_id):
        keyboard = [
            [InlineKeyboardButton("Баланс", callback_data='balance')],
            [InlineKeyboardButton("Каналы", callback_data='channels')],
            [InlineKeyboardButton("Админ", callback_data='admin')]
        ]
        reply_markup = InlineKeyboardMarkup(keyboard)
        update.message.reply_text('Вы подписаны на канал. Добро пожаловать!', reply_markup=reply_markup)
    else:
        update.message.reply_text(f'Пожалуйста, подпишитесь на канал: {CHANNEL_USERNAME}')

# Проверка подписки пользователя на канал
def check_subscription(user_id, channel=CHANNELS[0]):
    url = f"https://api.telegram.org/bot{TOKEN}/getChatMember?chat_id={channel}&user_id={user_id}"
    response = requests.get(url).json()
    status = response.get('result', {}).get('status', 'left')
    return status in ['member', 'administrator', 'creator']

# Обработчик кнопок
def button(update: Update, context: CallbackContext) -> None:
    query = update.callback_query
    query.answer()
    data = query.data
    user_id = query.from_user.id

    if data == 'balance':
        balance = user_balances.get(user_id, 0)
        query.edit_message_text(text=f"Ваш баланс: {balance} монет")
    elif data == 'channels':
        channels_text = "\n".join(CHANNELS)
        query.edit_message_text(text=f"Список доступных каналов:\n{channels_text}")
    elif data == 'admin':
        context.user_data['awaiting_code'] = True
        query.edit_message_text(text="Введите админский код:")

# Обработчик текстовых сообщений
def handle_message(update: Update, context: CallbackContext) -> None:
    user_id = update.message.from_user.id
    text = update.message.text

    if context.user_data.get('awaiting_code'):
        if text == ADMIN_CODE:
            context.user_data['admin'] = True
            context.user_data['awaiting_code'] = False
            update.message.reply_text("Код подтвержден. Вы вошли в админ-панель.\nВведите новый канал в формате @channel_username:")
        else:
            update.message.reply_text("Неверный код. Попробуйте снова.")
    elif context.user_data.get('admin'):
        if text.startswith('@'):
            CHANNELS.append(text)
            update.message.reply_text(f"Канал {text} добавлен. Введите следующий канал или /done для завершения.")
        elif text == '/done':
            context.user_data['admin'] = False
            update.message.reply_text("Админ-панель закрыта.")
    else:
        update.message.reply_text(text)

# Обновление баланса при подписке на новый канал
def check_and_update_balance(update: Update, context: CallbackContext) -> None:
    user_id = update.message.from_user.id
    balance_increase = 0

    for channel in CHANNELS:
        if check_subscription(user_id, channel):
            if user_id not in user_balances:
                user_balances[user_id] = 0
            user_balances[user_id] += 100
            balance_increase += 100

    if balance_increase > 0:
        update.message.reply_text(f"Вы подписались на новые каналы и получили {balance_increase} монет. Ваш новый баланс: {user_balances[user_id]} монет.")
    else:
        update.message.reply_text("Вы не подписаны на новые каналы.")

def main() -> None:
    # Создаем объект Updater и передаем ему токен вашего бота
    updater = Updater(TOKEN)

    # Получаем диспетчера для регистрации обработчиков
    dispatcher = updater.dispatcher

    # Регистрируем обработчик команд /start
    dispatcher.add_handler(CommandHandler("start", start))

    # Регистрируем обработчик кнопок
    dispatcher.add_handler(CallbackQueryHandler(button))

    # Регистрируем обработчик текстовых сообщений (эхо-бот)
    dispatcher.add_handler(MessageHandler(Filters.text & ~Filters.command, handle_message))

    # Проверка и обновление баланса при подписке на новый канал
    dispatcher.add_handler(MessageHandler(Filters.text & ~Filters.command, check_and_update_balance))

    # Запускаем бота
    updater.start_polling()

    # Работаем до тех пор, пока не будет прервана программа
    updater.idle()

if __name__ == '__main__':
    main()
