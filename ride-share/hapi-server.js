// Knex
const knex = require("knex")({
  client: "pg",
  connection: {
    host: "faraday.cse.taylor.edu", // PostgreSQL server
    user: "daisy_bell", // Your user name
    password: "kohifaga", // Your password
    database: "daisy_bell", // Your database name
  },
});

// Objection
const objection = require("objection");
objection.Model.knex(knex);

// Models
//const Account = require("./models/Account");
const Ride = require("./api/models/Ride");
const User = require("./api/models/User");
const Driver = require("./api/models/Driver");
const Passenger = require("./api/models/Passenger");
const Vehicle = require("./api/models/Vehicle");
const Vehicle_Type = require("./api/models/Vehicle-Type");
const Location = require("./api/models/Location");

// Hapi
const Joi = require("@hapi/joi"); // Input validation
const Hapi = require("@hapi/hapi"); // Server

const server = Hapi.server({
  host: "localhost",
  port: 3000,
  routes: {
    cors: true,
  },
});

async function init() {
  // Show routes at startup.
  await server.register(require("blipp"));

  // Output logging information.
  await server.register({
    plugin: require("hapi-pino"),
    options: {
      prettyPrint: true,
    },
  });

  // Configure routes.
  server.route([
    {
      method: "POST",
      path: "/accounts",
      config: {
        description: "Sign up for an account",
        validate: {
          payload: Joi.object({
            firstName: Joi.string().required(),
            lastName: Joi.string().required(),
            email: Joi.string()
              .email()
              .required(),
            phone: Joi.string()
              .required(),
            password: Joi.string().required(),
          }),
        },
      },
      handler: async (request, h) => {
        const existingAccount = await User.query()
          .where("email", request.payload.email)
          .first();
        if (existingAccount) {
          return {
            ok: false,
            msge: `Account with email '${request.payload.email}' is already in use`,
          };
        }

        const newAccount = await User.query().insert({
          firstName: request.payload.firstName,
          lastName: request.payload.lastName,
          email: request.payload.email,
          phone: request.payload.phone,
          password: request.payload.password,
        });

        if (newAccount) {
          return {
            ok: true,
            msge: `Created account '${request.payload.email}'`,
          };
        } else {
          return {
            ok: false,
            msge: `Couldn't create account with email '${request.payload.email}'`,
          };
        }
      },
    },

    {
      method: "GET",
      path: "/rides",
      config: {
        description: "Retrieve all rides",
      },
      handler: () => {
        return Ride.query()
          .withGraphFetched("to_locations")
          .withGraphFetched("from_locations")
          .withGraphFetched("vehicles");
      },
    },

    {
      method: "DELETE",
      path: "/accounts/{id}",
      config: {
        description: "Delete an account",
      },
      handler: (request, h) => {
        return Account.query()
          .deleteById(request.params.id)
          .then((rowsDeleted) => {
            if (rowsDeleted === 1) {
              return {
                ok: true,
                msge: `Deleted account with ID '${request.params.id}'`,
              };
            } else {
              return {
                ok: false,
                msge: `Couldn't delete account with ID '${request.params.id}'`,
              };
            }
          });
      },
    },

    {
      method: "POST",
      path: "/login",
      config: {
        description: "Log in",
      },
      handler: async (request, h) => {
        const user = await User.query()
          .withSchema("ride_share")
          .where("email", request.payload.email)
          .first();
        if (user) {
          return {
            ok: true,
            msge: `Logged in successfully as '${request.payload.email}'`,
            details: {
              id: user.user_id,
              firstName: user.firstName,
              lastName: user.lastName,
              email: user.email,
              isAdmin: user.isAdmin,
            },
          };
        } else {
          return {
            ok: false,
            msge: "Invalid email or password",
          };
        }
      },
    },

    {
      method: "POST",
      path: "/driver",
      config: {
        description: "Drivers in system",
      },
      handler: async (request, h) => {
        const driver = await Driver.query()
          .withSchema("ride_share")
          .where("user_id", request.payload.user_id)
          .first();
        if (driver) {
          return {
            ok: true,
            msge: `User with user id '${request.payload.user_id}' is a driver.`,
            details: {
              driver_id: driver.driver_id,
              user_id: driver.user_id,
              licenseNumber: driver.licenseNumber,
              licenseState: driver.LicenseState,
            },
          };
        } else {
          return {
            ok: false,
            msge: "User is not a driver",
          };
        }
      },
    },

    {
      method: "GET",
      path: "/rides/{userId}",
      config: {
        description: "Retrieve rides for a given user",
      },
      handler: (request) =>
        Passenger.query()
          .where("passenger_id", request.params.userId)
          .withGraphFetched("rides"),
    },

    {
      method: "DELETE",
      path: "/ride/{ride_id}",
      config: {
        description: "Delete an exisiting ride",
      },
      handler: (request, h) => {
        return Ride.query()
          .deleteById(request.params.ride_id)
          .then((rowsDeleted) => {
            if (rowsDeleted === 1) {
              return {
                ok: true,
                msge: `Deleted ride with ID '${request.params.ride_id}'`,
              };
            } else {
              return {
                ok: false,
                msge: `Couldn't delete ride with ID '${request.params.ride_id}'`,
              };
            }
          });
      },
    },

    {
      method: "POST",
      path: "/rides",
      config: {
        description: "Add a user to a ride",
      },
      handler: (request) =>
        Passenger.query().insert({
          passenger_id: request.payload.passenger_id,
          ride_id: request.payload.ride_id,
        }),
    },

    {
      method: "POST",
      path: "/passenger",
      config: {
        description: "Remove a user from a ride.",
      },
      handler: (request, h) =>
        Passenger.query()
          .where({
            passenger_id: request.payload.passenger_id,
            ride_id: request.payload.ride_id,
          })
          .delete()
          .then((result) => {
            console.log("RESULT", result);
            return {
              msge: `Removed User from Ride. PID: ${request.payload.passenger_id} - RID: ${request.payload.ride_id}`,
            };
          }),
    },

    {
      method: "GET",
      path: "/vehicles",
      config: {
        description: "Retrieve all vehicles",
      },
      handler: () => {
        return Vehicle.query();
      },
    },

    {
      method: "DELETE",
      path: "/vehicle/{vehicle_id}",
      config: {
        description: "Delete an exisiting vehicle",
      },
      handler: (request, h) => {
        return Vehicle.query()
          .deleteById(request.params.vehicle_id)
          .then((rowsDeleted) => {
            if (rowsDeleted === 1) {
              return {
                ok: true,
                msge: `Deleted vehicle with ID '${request.params.vehicle_id}'`,
              };
            } else {
              return {
                ok: false,
                msge: `Couldn't delete vehicle with ID '${request.params.vehicle_id}'`,
              };
            }
          });
      },
    },

    {
      method: "GET",
      path: "/vehicle_types",
      config: {
        description: "Retrieve all vehicle types",
      },
      handler: () => {
        return Vehicle_Type.query();
      },
    },

    {
      method: "DELETE",
      path: "/vehicle_type/{vehicle_type_id}",
      config: {
        description: "Delete an exisiting vehicle type",
      },
      handler: (request, h) => {
        return Vehicle_Type.query()
          .deleteById(request.params.vehicle_type_id)
          .then((rowsDeleted) => {
            if (rowsDeleted === 1) {
              return {
                ok: true,
                msge: `Deleted vehicle type with ID '${request.params.vehicle_type_id}'`,
              };
            } else {
              return {
                ok: false,
                msge: `Couldn't delete vehicle type with ID '${request.params.vehicle_type_id}'`,
              };
            }
          });
      },
    },

    {
      method: "GET",
      path: "/locations",
      config: {
        description: "Retrieve all locations",
      },
      handler: () => {
        return Location.query();
      },
    },

    {
      method: "DELETE",
      path: "/location/{location_id}",
      config: {
        description: "Delete an exisiting location",
      },
      handler: (request, h) => {
        return Location.query()
          .deleteById(request.params.location_id)
          .then((rowsDeleted) => {
            if (rowsDeleted === 1) {
              return {
                ok: true,
                msge: `Deleted location with ID '${request.params.location_id}'`,
              };
            } else {
              return {
                ok: false,
                msge: `Couldn't delete location with ID '${request.params.location_id}'`,
              };
            }
          });
      },
    },

    {
      method: "GET",
      path: "/users",
      config: {
        description: "Retrieve all users",
      },
      handler: () => {
        return User.query();
      },
    },

    {
      method: "DELETE",
      path: "/user/{user_id}",
      config: {
        description: "Delete an exisiting user",
      },
      handler: (request, h) => {
        return User.query()
          .deleteById(request.params.user_id)
          .then((rowsDeleted) => {
            if (rowsDeleted === 1) {
              return {
                ok: true,
                msge: `Deleted user with ID '${request.params.user_id}'`,
              };
            } else {
              return {
                ok: false,
                msge: `Couldn't delete user with ID '${request.params.user_id}'`,
              };
            }
          });
      },
    },

    {
      method: "GET",
      path: "/drivers",
      config: {
        description: "Retrieve all drivers",
      },
      handler: () => {
        return Driver.query()
          .withGraphFetched("users");
      },
    },

    {
      method: "DELETE",
      path: "/driver/{driver_id}",
      config: {
        description: "Delete an exisiting driver",
      },
      handler: (request, h) => {
        return Driver.query()
          .deleteById(request.params.driver_id)
          .then((rowsDeleted) => {
            if (rowsDeleted === 1) {
              return {
                ok: true,
                msge: `Deleted driver with ID '${request.params.driver_id}'`,
              };
            } else {
              return {
                ok: false,
                msge: `Couldn't delete driver with ID '${request.params.driver_id}'`,
              };
            }
          });
      },
    },

    {
      method: "GET",
      path: "/passengers",
      config: {
        description: "Retrieve all passengers",
      },
      handler: () => {
        return Passenger.query()
          .withGraphFetched("users")
          .withGraphFetched("rides");
      },
    },



  ]);

  // Start the server.
  await server.start();
}

process.on("unhandledRejection", (err) => {
  server.logger().error(err);
  process.exit(1);
});

// Go!
init();
