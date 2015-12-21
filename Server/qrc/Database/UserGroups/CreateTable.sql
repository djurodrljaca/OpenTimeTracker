CREATE TABLE UserGroups (
    id      INTEGER PRIMARY KEY AUTOINCREMENT
                    NOT NULL,
    name    TEXT    UNIQUE
                    NOT NULL
                    CHECK (name <> ''),
    enabled BOOLEAN NOT NULL
);
