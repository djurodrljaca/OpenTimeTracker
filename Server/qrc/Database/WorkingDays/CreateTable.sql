CREATE TABLE WorkingDays (
    id             INTEGER  PRIMARY KEY AUTOINCREMENT
                            NOT NULL,
    startTimestamp DATETIME CHECK (startTimestamp <> '')
                            NOT NULL,
    endTimestamp   DATETIME CHECK (endTimestamp <> '')
                            NOT NULL,
    CHECK (startTimestamp < endTimestamp)
);

CREATE INDEX index_WorkingDays_id ON WorkingDays (
    id
);
