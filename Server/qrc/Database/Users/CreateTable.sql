CREATE TABLE Users (
    id       INTEGER PRIMARY KEY AUTOINCREMENT
                     NOT NULL,
    name     TEXT    NOT NULL
                     CHECK (name <> '')
                     UNIQUE,
    password TEXT    CHECK ( (password ISNULL) OR
                             (password <> '') )
                     UNIQUE
);
CREATE INDEX [] ON Users (
    id,
    name,
    password
);
