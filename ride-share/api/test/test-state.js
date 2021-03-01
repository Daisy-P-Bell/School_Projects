/*
	Ride Share ORM
    State ORM Test
	COS 243
	Daisy Bell & Matthias Dean
*/

const { knex, Model } = require("../db");
const State = require("../models/State.js");

State.query().withSchema('ride_share')
    .select('abbreviation')
    .withGraphFetched('locations')
    .then(state => console.log(state))
    .catch(error => console.log(error.message))
    .then(() => knex.destroy());