# Ride Share ERD

COS 243

Daisy Bell & Matthias Dean

1. The ERD allows multiple drivers to be authorized to drive multiple cars through the `Authorization` table. This table is classified as an associative table that goes between the `Driver` table and the `Vehicle` table. 
2. “A ride may have one or more drivers assigned. All drivers for a ride must be authorized to drive the vehicle for that ride.” This means that could be many drivers to one ride, so you need the `Drivers` table for a many-to-one relationship.
3. The `Ride` table uses the `id` from the `Vehicle` table, which takes the `vehicleId` from the `Authorization` table, which means that the driver has to be authorized in order to drive that vehicle. The *Driver ID* is linked to the *Vehicle ID*, so it would have to be the same vehicle. 
4. In an effort to save database space and complexity of data entries, we think it’s a better option to use the `fromLocationId` and the `toLocationId` to calculate that value when we need it. 
5. As far as the ERD is concerned, it doesn’t enforce any rule that makes you follow the capacity limit. We can add a data constraint for the value when it’s attempted to be put in the table that prevents the passenger number from exceeding the capacity within the vehicle table. 
6. No, we don’t think it should be stored in the database because that’s a lot of unnecessary data to be kept in the database that can be calculated at runtime. The ride fee is charged to each user, and the fuel (price * miles per gallon * distance) would be split evenly between passengers. 