/*
	Ride Share ORM
    Ride ORM Test
	COS 243
	Daisy Bell & Matthias Dean
*/

const { knex, Model } = require("../db");
const Ride = require("../models/Passenger.js");

Ride.query().withSchema('ride_share')
    .where("passenger_id", 6)
    .andWhere("ride_id", 9)
    .del()
    .catch(error => console.log(error.message))
    .then(() => knex.destroy());
/*
    .select('ride_id')
    .withGraphFetched('rides')
    .then(ride => console.log(ride))
    .catch(error => console.log(error.message))
    .then(() => knex.destroy());
    */