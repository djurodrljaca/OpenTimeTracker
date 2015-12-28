SELECT * FROM Schedules
WHERE (:startOfWorkingDay <= startTimestamp) AND (startTimestamp <= :endOfWorkingDay) AND
      (:startOfWorkingDay <= endTimestamp) AND (endTimestamp <= :endOfWorkingDay) AND
      (userId == :userId)
ORDER BY startTimestamp ASC;
