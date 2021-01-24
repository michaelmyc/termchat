#ifndef SETTINGS_PROCESSED
#define SETTINGS_PROCESSED

#define MSG_LIMIT 280
#define LOG_LIMIT 512
#define USERNAME_LIMIT 32
#define ROOMNAME_LIMIT 32
#define NETWORK_LIMIT 128

typedef struct Message {
  char user[USERNAME_LIMIT];
  char room[ROOMNAME_LIMIT];
  char network[NETWORK_LIMIT];
  char message[MSG_LIMIT];
} Message;

#endif