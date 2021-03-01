/*
	Ride Share ORM
    Vehicle ORM Test
	COS 243
	Daisy Bell & Matthias Dean
*/

const { knex, Model } = require("../db");
const Vehicle = require("../models/Vehicle.js");

Vehicle.query().withSchema('ride_share')
    .select('vehicle_id')
    .withGraphFetched('vehicle_types')
    .then(vehicle => console.log(vehicle))
    .catch(error => console.log(error.message))
    .then(() => knex.destroy());