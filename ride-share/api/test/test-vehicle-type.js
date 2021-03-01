/*
	Ride Share ORM
    Vehicle Type ORM Test
	COS 243
	Daisy Bell & Matthias Dean
*/

const { knex, Model } = require("../db");
const VehicleType = require("../models/Vehicle-Type.js");

VehicleType.query().withSchema('ride_share')
    .select('vehicle_type_id')
    .withGraphFetched('vehicles')
    .then(vehicle => console.log(vehicle))
    .catch(error => console.log(error.message))
    .then(() => knex.destroy());