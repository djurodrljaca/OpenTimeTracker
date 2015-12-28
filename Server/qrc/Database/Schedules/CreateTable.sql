CREATE TABLE Schedules (
    id             INTEGER  PRIMARY KEY AUTOINCREMENT
                            NOT NULL,
    userId         INTEGER  REFERENCES Users (id)
                            NOT NULL,
    startTimestamp DATETIME CHECK (startTimestamp <> '')
                            NOT NULL,
    endTimestamp   DATETIME CHECK (endTimestamp <> '')
                            NOT NULL,
    CHECK (startTimestamp < endTimestamp)
);

CREATE INDEX index_Schedules_search ON Schedules (
    userId,
    startTimestamp,
    endTimestamp
);
