Noun-Verb Analysis

For the three specifications above, the raw noun list could be:

Raw Candidate List
Trader
Stock
Order
Buy Order
Sell Order
Quantity
Price
Cash
Portfolio
Holding
Transaction
Market
Market Price
Simulation
Simulation Clock
Simulation Tick
Market Event
Event Probability
Market Parameter
Price Alert
Notification
Notification Service
Account
Login
Balance
Transaction Value
Market Statistics
Four Filters

We'll use these four filters:

Duplicate/Synonym Filter
Remove nouns that represent the same concept.
Attribute Filter
Remove nouns that are properties of another class rather than independent classes.
Out-of-Scope Filter
Remove concepts outside the responsibility of our simulator.
Implementation/UI Filter
Remove concepts that are merely implementation details or interface elements.
Applying the Filters
Candidate	Filter	Reason
Trader	Survivor	Independent system actor/domain entity
Stock	Survivor	Core domain entity
Order	Survivor	Core domain entity
Buy Order	Duplicate/Specialization	Model as Order type rather than separate class
Sell Order	Duplicate/Specialization	Model as Order type rather than separate class
Quantity	Attribute	Property of Order
Price	Attribute	Property of Stock/Order
Cash	Attribute	Part of Account
Portfolio	Survivor	Core domain entity
Holding	Survivor	Connects Portfolio and Stock
Transaction	Survivor	Records executed trade
Market	Survivor	Core domain concept
Market Price	Duplicate	Represented by Stock.currentPrice
Simulation	Survivor	Controls simulation
Simulation Clock	Implementation/External	Model as scheduled actor
Simulation Tick	Attribute/Event	Operation/event rather than core class
Market Event	Survivor	Represents simulated events
Event Probability	Attribute	Property of simulation/event
Market Parameter	Attribute	Property of Simulation
Price Alert	Survivor	User-defined domain object
Notification	Survivor	Represents generated notification
Notification Service	External	External service/actor
Account	Survivor	Trader's virtual-money account
Login	Implementation/UI	Authentication operation
Balance	Attribute	Property of Account
Transaction Value	Attribute	Calculated property
Market Statistics	Attribute/Derived	Can be calculated by Market
Notification message	Attribute	Property of Notification
Surviving Classes

Trader

Account

Stock

Market

Order

Portfolio

Holding

Transaction

Simulation

MarketEvent

PriceAlert

Notification