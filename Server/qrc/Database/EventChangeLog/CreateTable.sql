CREATE TABLE EventChangeLog (
    id        INTEGER  PRIMARY KEY AUTOINCREMENT
                       NOT NULL,
    eventId   INTEGER  REFERENCES Events (id)
                       NOT NULL,
    timestamp DATETIME NOT NULL
                       CHECK (timestamp <> ''),
    fieldName TEXT     NOT NULL
                       CHECK ( (fieldName == 'timestamp') OR
                               (fieldName == 'type') OR
                               (fieldName == 'enabled') ),
    fromValue          NOT NULL,
    toValue            NOT NULL,
    userId    INTEGER  REFERENCES Users (id)
                       NOT NULL,
    comment   TEXT     NOT NULL
                       CHECK (comment <> ''),
    CHECK (fromValue <> toValue)
);
