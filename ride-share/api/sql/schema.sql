/*
	Ride Share Schema 
    Database Schema
	COS 243
	Daisy Bell & Matthias Dean
*/

create table "User"
(
    user_id     serial                not null
        constraint user_pk
            primary key,
    "firstName" varchar(40)           not null,
    "lastName"  varchar(40)           not null,
    email       varchar(40)           not null,
    password    varchar(40)           not null,
    phone       varchar(40)           not null,
    "isAdmin"   boolean default false not null
);

create unique index user_password_uindex
    on "User" (password);

create table "Vehicle-Type"
(
    vehicle_type_id serial      not null
        constraint "vehicle-type_pk"
            primary key,
    type            varchar(40) not null
);

create table "State"
(
    abbreviation varchar(40) not null
        constraint state_pk
            primary key,
    name         varchar(40) not null
);

create table "Driver"
(
    driver_id       serial      not null
        constraint driver_pk
            primary key,
    user_id         integer     not null
        constraint user__id
            references "User",
    "licenseNumber" varchar(40) not null,
    "licenseState"  varchar(40) not null
        constraint driver_state_abbreviation_fk
            references "State"
);

create table "Vehicle"
(
    vehicle_id      serial           not null
        constraint vehicle_pk
            primary key,
    make            varchar(40)      not null,
    model           varchar(40)      not null,
    color           varchar(40)      not null,
    vehicle_type_id integer          not null
        constraint "vehicle_vehicle-type_vehicle_type_id_fk"
            references "Vehicle-Type",
    capacity        integer          not null,
    mpg             double precision not null,
    licence_state   varchar(40)      not null
        constraint vehicle_state_abbreviation_fk
            references "State",
    licence_plate   varchar(40)      not null
);

create table "Authorization"
(
    driver_id  serial not null
        constraint authorization_driver_driver_id_fk
            references "Driver",
    vehicle_id serial not null
        constraint authorization_vehicle_vehicle_id_fk
            references "Vehicle",
    constraint authorization_pk
        unique (driver_id, vehicle_id)
);

create table "Location"
(
    location_id serial      not null
        constraint location_pk
            primary key,
    name        varchar(40) not null,
    address     varchar(40) not null,
    city        varchar(40) not null,
    state       varchar(40) not null
        constraint location_state_abbreviation_fk
            references "State",
    zip_code    varchar(40) not null
);

create table "Ride"
(
    ride_id          serial           not null
        constraint ride_pk
            primary key,
    date             date             not null,
    time             time             not null,
    distance         double precision not null,
    fuel_price       double precision not null,
    fee              double precision not null,
    vehicle_id       integer          not null
        constraint ride_vehicle_vehicle_id_fk
            references "Vehicle",
    from_location_id integer          not null
        constraint ride_location_location_id_fk
            references "Location",
    to_location_id   integer          not null
        constraint ride_location_location_id_fk_2
            references "Location"
);

create table "Passenger"
(
    passenger_id serial  not null
        constraint passengerid__userid
            references "User",
    ride_id      integer not null
        constraint passenger_ride_ride_id_fk
            references "Ride",
    constraint passenger_pk
        unique (passenger_id, ride_id)
);

create table "Drivers"
(
    driver_id serial not null
        constraint drivers_driver_driver_id_fk
            references "Driver",
    ride_id   serial not null
        constraint drivers_ride_ride_id_fk
            references "Ride",
    constraint drivers_pk
        unique (driver_id, ride_id)
);

