CREATE TABLE Users (
    id       BIGINT PRIMARY KEY
                    UNIQUE,
    name     TEXT   UNIQUE,
    password TEXT   UNIQUE
);
