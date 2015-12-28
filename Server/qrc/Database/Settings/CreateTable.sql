CREATE TABLE Settings (
    name  TEXT UNIQUE
               NOT NULL
               CHECK (name <> ''),
    value
);
