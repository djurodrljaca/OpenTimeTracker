CREATE TABLE Settings (
    name  TEXT UNIQUE
               NOT NULL
               CHECK (name <> ''),
    value
);

CREATE INDEX index_Settings_name ON Settings (
    name
);
