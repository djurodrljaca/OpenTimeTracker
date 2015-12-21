CREATE TABLE Users (
    id       INTEGER PRIMARY KEY AUTOINCREMENT
                     NOT NULL,
    name     TEXT    NOT NULL
                     CHECK (name <> ''),
    password TEXT    CHECK ( ( (password NOTNULL) AND
                               (password <> '') AND
                               (enabled = 1) ) OR
                             ( (password ISNULL) AND
                               (enabled = 0) ) )
                     UNIQUE,
    enabled  BOOLEAN NOT NULL
);
