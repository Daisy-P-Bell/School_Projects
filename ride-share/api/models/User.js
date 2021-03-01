/*
    Ride Share ORM
    User Model
    COS 243
    Daisy Bell & Matthias Dean
*/

const { Model } = require("../db");

class User extends Model {
  static get tableName() {
    return "User";
  }
  static get idColumn() {
    return "user_id";
  }
  static get relationMappings() {
    return {
      drivers: {
        relation: Model.HasManyRelation,
        modelClass: require("./Driver"),
        join: {
          from: "User.user_id",
          to: "Driver.user_id",
        },
      },
      passengers: {
        relation: Model.HasManyRelation,
        modelClass: require("./Passenger"),
        join: {
          from: "User.user_id",
          to: "Passenger.passenger_id",
        },
      },
    };
  }
}

module.exports = User;
