SELECT * FROM Events
WHERE (timestamp >= :startTimestamp) AND (timestamp <= :endTimestamp) AND (userId == :userId);
