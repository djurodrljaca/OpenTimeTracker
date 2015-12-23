CREATE TABLE Events (
    id        INTEGER  PRIMARY KEY AUTOINCREMENT
                       NOT NULL,
    timestamp DATETIME NOT NULL
                       CHECK (timestamp <> ''),
    userId    INTEGER  REFERENCES Users (id)
                       NOT NULL,
    type      INTEGER  NOT NULL
                       CHECK ( (type >= 1) AND
                               (type <= 4) ),
    enabled   BOOLEAN  NOT NULL
                       CHECK ( (enabled == 0) OR
                               (enabled == 1) )
);

CREATE INDEX index_Events_id ON Events (
    id
);

CREATE INDEX index_Events_timestamp ON Events (
    timestamp
);

CREATE INDEX index_Events_userId ON Events (
    userId
);
