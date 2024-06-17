-- Keep a log of any SQL queries you execute as you solve the mystery.
--the theft took place on July 28, 2023 and that it took place on Humphrey Street.

--let's find the record for the crime in the crime_scene_reports table:
SELECT * FROM crime_scene_reports WHERE street = 'Humphrey Street';

--it says we have three witnesses who were present at the time, 10:15am, they all mention the bakery...

--let's look closer at what they said, maybe in transcripts from interviews:
SELECT transcript FROM interviews WHERE year = 2023 AND month = 7 AND day = 28;

/*okay here's some suspect lines:
    within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away
     Earlier this morning, before I arrived at Emma's bakery,
    I was walking by the ATM on Leggett Street and saw the thief
    there withdrawing some money.

    As the thief was leaving the bakery,
    they called someone who talked to them for less than a minute
    earliest flight out of Fiftyville tomorrow. The thief then asked
    the person on the other end of the phone to purchase the flight ticket.

to do next:
    -look at the bakery security logs 10 min after crime and grab some license plates
    -check atm records for any possible evidence
    -phone records from around 10:15 - 10:25
    -flight purchase records/flights that are earliest to find city! */

--retreives suspect license plates from bakery
SELECT license_plate FROM bakery_security_logs
    WHERE hour = 10 AND minute > 14 AND minute < 30 AND day = 28;

--retrieves atm record from leggett Street on the morning of crime
SELECT *
FROM atm_transactions
WHERE atm_location = 'Leggett Street'
AND transaction_type = 'withdraw'
AND month = 7
AND day = 28;

--checks phone records for 7/28 to retrieve numbers to associate with accomplice/thief
SELECT *
FROM phone_calls
WHERE month = 7
AND day = 28
AND duration < 60;

--retrieves flights out of Fiftyville, but first find fiftyville airport, orders by earliest
SELECT *
FROM flights
WHERE origin_airport_id = (
    SELECT id
    FROM airports
    WHERE city = 'Fiftyville'
)
AND day = 29
AND month = 7
AND year = 2023
ORDER BY hour, minute;

-- earliest flight is id = 36
--retrieves all passport numbers on flight taken by THIEF!!!
SELECT passport_number
FROM passengers
WHERE flight_id = 36

--lets find destination airport:
SELECT city
FROM airports
WHERE id = (
    SELECT destination_airport_id
    FROM flights
    WHERE id = 36
);

--okay we've got enough to start cross referencing records to get a small pool of people.
--let's see who has a passport number that matches the flight:

SELECT *
FROM people
WHERE passport_number IN (
    SELECT passport_number
    FROM passengers
    WHERE flight_id = 36
)

--OKAY KENNY or Doris, Doris is the receiving call from phone records, so are they the accomplice? Hmmm
/*
id  |     caller     |    receiver    | year | month | day | duration |
+-----+----------------+----------------+------+-------+-----+----------+
| 279 | (826) 555-1652 | (066) 555-9701 | 2023 | 7     | 28  | 55       |
+--------+-------+----------------+-----------------+---------------+
|   id   | name  |  phone_number  | passport_number | license_plate |
+--------+-------+----------------+-----------------+---------------+
| 953679 | Doris | (066) 555-9701 | 7214083635      | M51FA04       |
*/

--LET'S make one huge nested inquiry with all of our data now!
SELECT *
FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE id IN (
    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN (
        SELECT account_number
        FROM atm_transactions
        WHERE atm_location = 'Leggett Street'
        AND transaction_type = 'withdraw'
        AND month = 7
        AND day = 28
    )
)
AND
phone_number IN (
    SELECT caller
    FROM phone_calls
    WHERE month = 7
    AND day = 28
    AND duration < 60
)
AND
passport_number IN (
    SELECT passport_number
    FROM passengers
    WHERE flight_id = 36
)
AND
license_plate IN (
    SELECT license_plate
    FROM bakery_security_logs
    WHERE hour = 10
    AND minute > 14
    AND minute < 26
    AND day = 28
);

/* above query returns BRUCE:
|   id   | name  |  phone_number  | passport_number | license_plate | account_number | person_id | creation_year |
+--------+-------+----------------+-----------------+---------------+----------------+-----------+---------------+
| 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       | 49610011       | 686048    | 2010          |

let's cross reference phone log to find accomplice
*/

SELECT *
FROM people
WHERE phone_number = (
    SELECT receiver
    FROM phone_calls
    WHERE id = 233
);

-- Robin!!!




