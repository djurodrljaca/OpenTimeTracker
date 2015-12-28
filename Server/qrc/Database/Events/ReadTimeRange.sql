SELECT * FROM Events
WHERE (:startTimestamp <= timestamp) AND (timestamp <= :endTimestamp) AND (userId == :userId);
