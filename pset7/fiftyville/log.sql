SELECT * FROM crime_scene_reports WHERE street = "Chamberlin Street";
    --Output: 
    --295|2020|7|28|Chamberlin Street|Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse. 
    --Interviews were conducted today with three witnesses who were present at the time - each of their interview transcripts 
    --mentions the courthouse.

SELECT * FROM interviews WHERE transcript LIKE "%courthouse%" AND day = 28;

    -- 161|Ruth|2020|7|28|Sometime within ten minutes of the theft, I saw the thief get into a car in the 
    --courthouse parking lot and drive away. If you have security footage from the courthouse parking lot, 
    --you might want to look for cars that left the parking lot in that time frame.
--
    -- 162|Eugene|2020|7|28|I don't know the thief's name, but it was someone I recognized. 
    --Earlier this morning, before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there 
    --withdrawing some money.

    --163|Raymond|2020|7|28|As the thief was leaving the courthouse, they called someone who talked to them for less than a minute. 
    --In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. 
    --The thief then asked the person on the other end of the phone to purchase the flight ticket.

--ATM withdraw -> atm account
SELECT * FROM atm_transactions WHERE atm_location = "Fifer Street" AND transaction_type = "withdraw" AND  day = 28 AND month = 7;
    
    --246|28500762|2020|7|28|Fifer Street|withdraw|48
    --264|28296815|2020|7|28|Fifer Street|withdraw|20
    --266|76054385|2020|7|28|Fifer Street|withdraw|60
    --267|49610011|2020|7|28|Fifer Street|withdraw|50
    --269|16153065|2020|7|28|Fifer Street|withdraw|80
    --288|25506511|2020|7|28|Fifer Street|withdraw|20
    --313|81061156|2020|7|28|Fifer Street|withdraw|30
    --336|26013199|2020|7|28|Fifer Street|withdraw|35

-- courthouse exit time -> license plate 
SELECT * FROM courthouse_security_logs WHERE year = 2020 AND month = 7 AND day = 28 and activity = "exit";

--INTERSECT of courthouse and atm

    --extract name from all atm transactions
SELECT name FROM people WHERE id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE atm_location = "Fifer Street" AND transaction_type = "withdraw" AND  day = 28 AND month = 7))
INTERSECT
SELECT name FROM people WHERE license_plate IN (SELECT license_plate FROM courthouse_security_logs WHERE year = 2020 AND month = 7 AND day = 28 and activity = "exit");

SELECT phone_number FROM people WHERE id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE atm_location = "Fifer Street" AND transaction_type = "withdraw" AND  day = 28 AND month = 7))
INTERSECT
SELECT phone_number FROM people WHERE license_plate IN (SELECT license_plate FROM courthouse_security_logs WHERE year = 2020 AND month = 7 AND day = 28 and activity = "exit");

--phone calls based on id from INTERSECT
SELECT * FROM phone_calls WHERE caller IN (
    SELECT phone_number FROM people WHERE id IN (
        SELECT person_id FROM bank_accounts WHERE account_number IN (
            SELECT account_number FROM atm_transactions WHERE atm_location = "Fifer Street" AND transaction_type = "withdraw" AND  day = 28 AND month = 7
            )
        )
INTERSECT
SELECT phone_number FROM people WHERE license_plate IN (
    SELECT license_plate FROM courthouse_security_logs WHERE year = 2020 AND month = 7 AND day = 28 and activity = "exit"
    )
) AND day = 28 AND duration < 60;

        --233|(367) 555-5533|(375) 555-8161|2020|7|28|45
        --254|(286) 555-6063|(676) 555-6554|2020|7|28|43
        --255|(770) 555-1861|(725) 555-3243|2020|7|28|49

--Extracting person info from filtered phone number

    --set 1: Ernest (367) 555-5533 and Berthold (375) 555-8161
    --set 2: Madison (286) 555-6063 and James (676) 555-6554
    --set 3: Russell (770) 555-1861 and Philip (725) 555-3243

--686048|Ernest   |(367) 555-5533     |5773159633 |94KL13X
--864400|Berthold |(375) 555-8161     |           |4V16VO0

--449774|Madison  |(286) 555-6063     |1988161715 |1106N58
--250277|James    |(676) 555-6554     |2438825627 |Q13SVG6

--514354|Russell  |(770) 555-1861     |3592750733 |322W7JE
--847116|Philip   |(725) 555-3243     |3391710505 |GW362R6

SELECT name FROM people WHERE phone_number = ...;
--airport info
SELECT * FROM flights JOIN airports ON flights.origin_airport_id = airports.id WHERE city = "Fiftyville" AND day = 28 and month = 7 ORDER BY hour ASC;

--ORDER BY HOUR
--6 |8|5|2020|7|28  |13|49|8|CSF|Fiftyville Regional Airport|Fiftyville
--35|8|4|2020|7|28  |16|16|8|CSF|Fiftyville Regional Airport|Fiftyville
--1 |8|7|2020|7|28  |17|50|8|CSF|Fiftyville Regional Airport|Fiftyville
--34|8|5|2020|7|28  |17|20|8|CSF|Fiftyville Regional Airport|Fiftyville
--17|8|4|2020|7|28  |20|16|8|CSF|Fiftyville Regional Airport|Fiftyville

SELECT id FROM flights WHERE origin_airport_id = 8 AND month = 7 and day >= 28 ORDER BY hour ASC;

SELECT * FROM passengers WHERE flight_id IN (SELECT id FROM flights WHERE origin_airport_id = 8 AND month = 7 and day >= 28 ORDER BY hour ASC) AND passport_number = ;

--36|5773159633|4A
--36|1988161715|6D
--10|2438825627|7C
--18|3592750733|4C
--54|3592750733|6C
--10|3391710505|2A

SELECT * FROM flights WHERE id = ;

--36|8|4|2020|7|29|8|20
--10|8|4|2020|7|30|13|55
--18|8|6|2020|7|29|16|0
--54|8|5|2020|7|30|10|19

SELECT * FROM airports WHERE id = ;

--4|LHR|Heathrow Airport|London
--5|DFS|Dallas/Fort Worth International Airport|Dallas
--6|BOS|Logan International Airport|Boston

--