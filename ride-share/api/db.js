/*
    Ride Share ORM
    COS 243
    Daisy Bell & Matthias Dean
*/

const knex = require("knex")({
  client: "pg",
  connection: {
    host: "faraday.cse.taylor.edu",
    user: "daisy_bell",
    password: "kohifaga",
    database: "daisy_bell",
  },
  searchPath: ["ride_share", "public"],
});

const Objection = require("objection");
const Model = Objection.Model;
Model.knex(knex);

module.exports = { knex, Model };

/*
knex.withSchema('ride_share')   
    .select('firstName')
    .from('User')
    .then(result => console.log(result))
    .then(() => knex.destroy());
*/
