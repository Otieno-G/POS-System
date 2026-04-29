
Below are some of the things my system  does;
1.Transaction Management

   Itemized Billing: Calculates the subtotal based on product prices and quantities.

Tax & Discount Logic: Automatically applies standard tax rates (e.g., VAT) and calculates discounts for eligible purchases.

Change Calculation: Computes the exact balance due to the customer for cash transactions.

2. Object-Oriented Design (OOP)

    Encapsulation: Uses classes to keep product data and payment logic separate and secure.

    Inheritance & Polymorphism: Designed to handle multiple payment methods (Cash, Digital/M-Pesa) through a unified payment interface.

3. Persistent Data (File Handling)

    Digital Receipts: Instead of just printing to the console, the system uses fstream to generate a permanent .txt file for every successful transaction.

    Audit Trail: Saves transaction details (Date, Time, Amount, and Payment Method) which can be used for basic business reporting.

4. Error Handling & Validation

    Input Sanitization: Prevents the program from crashing if a user enters a letter instead of a number.

    Fund Validation: Ensures that a transaction cannot be completed unless the amount paid is greater than or equal to the total cost.
