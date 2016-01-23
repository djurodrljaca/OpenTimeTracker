SELECT startTimestamp, endTimestamp FROM WorkingDays
WHERE ((startTimestamp <= :timestamp) AND (:timestamp <= endTimestamp))
ORDER BY id DESC
LIMIT 1;
