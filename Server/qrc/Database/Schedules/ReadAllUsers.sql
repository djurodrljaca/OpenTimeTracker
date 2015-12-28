SELECT * FROM Schedules
WHERE (:startOfWorkingDay <= startTimestamp) AND (startTimestamp <= :endOfWorkingDay) AND
      (:startOfWorkingDay <= endTimestamp) AND (endTimestamp <= :endOfWorkingDay)
ORDER BY userId, startTimestamp ASC;
