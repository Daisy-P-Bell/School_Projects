/*
	Ride Share ORM
    Ride ORM Test
	COS 243
	Daisy Bell & Matthias Dean
*/

const { knex, Model } = require("../db");
const Ride = require("../models/Ride.js");

Ride.query()
    .withGraphFetched("to_locations")
    .withGraphFetched("from_locations")
    .withGraphFetched("vehicles")
    .withGraphFetched("passengers")
    .then(ride => console.log(ride))
    .catch(error => console.log(error.message))
    .then(() => knex.destroy());
/*
Ride.query().withSchema('ride_share')
    .select('ride_id')
    .withGraphFetched('to_locations')
    .withGraphFetched('from_locations')
    .then(ride => console.log(ride))
    .catch(error => console.log(error.message))
    .then(() => knex.destroy());
    */