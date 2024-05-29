#!/bin/bash

gcc -pthread -Wall -Wextra -Werror server.c -o server

if [ $? -eq 0 ]; then
    echo "Компиляция успешно завершена. Исполняемый файл 'server' создан."
else
    echo "Ошибка при компиляции."
fi

gcc -pthread -Wall -Wextra -Werror client.c -o client

if [ $? -eq 0 ]; then
    echo "Компиляция успешно завершена. Исполняемый файл 'client' создан."
else
    echo "Ошибка при компиляции."
fi
