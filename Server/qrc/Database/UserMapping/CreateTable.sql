CREATE TABLE UserMapping (
    id          INTEGER PRIMARY KEY AUTOINCREMENT
                        NOT NULL,
    userGroupId INTEGER REFERENCES UserGroups (id)
                        NOT NULL,
    userId      INTEGER REFERENCES Users (id)
                        NOT NULL,
    UNIQUE (
        userGroupId,
        userId
    )
);
