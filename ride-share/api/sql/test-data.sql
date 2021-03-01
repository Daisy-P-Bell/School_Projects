/*
	Ride Share Schema 
	Test Data
	COS 243
	Daisy Bell & Matthias Dean
*/

-- Authorization
INSERT INTO ride_share."Authorization" (driver_id, vehicle_id) VALUES (2, 2);
INSERT INTO ride_share."Authorization" (driver_id, vehicle_id) VALUES (3, 3);
INSERT INTO ride_share."Authorization" (driver_id, vehicle_id) VALUES (4, 4);
-- Driver
INSERT INTO ride_share."Driver" (driver_id, user_id, "licenseNumber", "licenseState") VALUES (2, 1, '123', 'IN');
INSERT INTO ride_share."Driver" (driver_id, user_id, "licenseNumber", "licenseState") VALUES (3, 2, '456', 'OH');
INSERT INTO ride_share."Driver" (driver_id, user_id, "licenseNumber", "licenseState") VALUES (4, 3, '789', 'NY');
-- Drivers
INSERT INTO ride_share."Drivers" (driver_id, ride_id) VALUES (2, 7);
INSERT INTO ride_share."Drivers" (driver_id, ride_id) VALUES (3, 8);
INSERT INTO ride_share."Drivers" (driver_id, ride_id) VALUES (4, 9);
-- Location
INSERT INTO ride_share."Location" (location_id, name, address, city, state, zip_code) VALUES (5, 'name1', 'address1', 'city1', 'IN', '12345');
INSERT INTO ride_share."Location" (location_id, name, address, city, state, zip_code) VALUES (6, 'name2', 'address2', 'city2', 'OH', '23456');
INSERT INTO ride_share."Location" (location_id, name, address, city, state, zip_code) VALUES (7, 'name3', 'address3', 'city3', 'NY', '34567');
-- Passenger
INSERT INTO ride_share."Passenger" (passenger_id, ride_id) VALUES (1, 7);
INSERT INTO ride_share."Passenger" (passenger_id, ride_id) VALUES (2, 8);
INSERT INTO ride_share."Passenger" (passenger_id, ride_id) VALUES (3, 9);
-- Ride
INSERT INTO ride_share."Ride" (ride_id, date, time, distance, fuel_price, fee, vehicle_id, from_location_id, to_location_id) VALUES (7, '2020-09-01', '12:01:00', 1, 2.00, 10, 2, 5, 6);
INSERT INTO ride_share."Ride" (ride_id, date, time, distance, fuel_price, fee, vehicle_id, from_location_id, to_location_id) VALUES (8, '2020-09-02', '12:02:00', 2, 3.00, 15, 3, 6, 7);
INSERT INTO ride_share."Ride" (ride_id, date, time, distance, fuel_price, fee, vehicle_id, from_location_id, to_location_id) VALUES (9, '2020-09-03', '12:03:00', 3, 4.00, 20, 4, 7, 5);
-- State
INSERT INTO ride_share."State" (abbreviation, name) VALUES ('IN', 'Indiana');
INSERT INTO ride_share."State" (abbreviation, name) VALUES ('OH', 'Ohio');
INSERT INTO ride_share."State" (abbreviation, name) VALUES ('NY', 'New York');
-- User
INSERT INTO ride_share."User" (user_id, "firstName", "lastName", email, password, phone, "isAdmin") VALUES (1, 'Daisy', 'Bell', 'no@gmail.com', 'password99', '123-456-7897', false);
INSERT INTO ride_share."User" (user_id, "firstName", "lastName", email, password, phone, "isAdmin") VALUES (2, 'Matthias', 'Dean', 'nah@gmail.com', 'password1234', '555-555-5555', true);
INSERT INTO ride_share."User" (user_id, "firstName", "lastName", email, password, phone, "isAdmin") VALUES (3, 'Dr.', 'Nurk', 'nope@gmail.com', 'password123', '999-999-9999', false);
-- Vehicle
INSERT INTO ride_share."Vehicle" (make, model, color, vehicle_type_id, capacity, mpg, licence_state, licence_plate) VALUES ('Ford', 'Fusion', 'Silver', 1, 5, 29.1, 'IN', 'BB 3997');
INSERT INTO ride_share."Vehicle" (make, model, color, vehicle_type_id, capacity, mpg, licence_state, licence_plate) VALUES ('Toyota', 'Avalon', 'White', 2, 5, 24.1, 'IN', 'D135AM');
INSERT INTO ride_share."Vehicle" (make, model, color, vehicle_type_id, capacity, mpg, licence_state, licence_plate) VALUES ('Ford', 'Mustang GT500', 'Charcoal', 1, 2, 14, 'OH', 'MATEO');
-- Vehicle-Type
INSERT INTO ride_share."Vehicle-Type" (vehicle_type_id, type) VALUES  (1, 'car');
INSERT INTO ride_share."Vehicle-Type" (vehicle_type_id, type) VALUES  (2, 'van');
INSERT INTO ride_share."Vehicle-Type" (vehicle_type_id, type) VALUES  (3, 'SUV');