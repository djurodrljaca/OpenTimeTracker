CREATE TABLE UserGroups (
    id      INTEGER PRIMARY KEY AUTOINCREMENT
                    NOT NULL,
    name    TEXT    UNIQUE
                    NOT NULL,
    enabled BOOLEAN NOT NULL
);
