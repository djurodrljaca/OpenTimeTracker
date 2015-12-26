UPDATE Users
SET password = :password
WHERE (id == :id);
