# Stock Market Simulator

A console-based virtual stock trading simulator written in C++, built as an
OOP (Object-Oriented Programming) mini-project. It lets a trader open a
virtual account, buy and sell simulated stocks, track a portfolio, set price
alerts, and watch prices move under a simple market simulation engine — all
without touching real money or real markets.

See [`problem-statement.md`](problem-statement.md) for the original problem
statement, [`crc-cards.md`](crc-cards.md) for the class design (CRC cards),
and [`use-cases.md`](use-cases.md) / the `.puml` diagrams for the UML design.

## Features

- Create a trader account with a starting virtual balance
- View live (simulated) stock prices in the market
- Buy and sell stocks, with validation (funds, holdings, market open/closed)
- Track a portfolio with live valuation
- Full order history (every buy/sell attempt, accepted or rejected) and
  transaction history (every completed trade)
- Set price alerts on any stock (rises to/above or falls to/below a target)
  and get notified the moment they trigger
- An admin-style simulation engine: start/stop the simulation and advance it
  tick by tick, each tick applying a random market event (bull run, bear
  crash, volatility spike, stable drift) to every stock's price
- Market open/close control, gating whether trades are allowed

## Class design

Every class below maps directly to a CRC card in `crc-cards.md`:

| Class | Responsibility |
|---|---|
| `Stock` | A single stock's symbol, company name, current/previous price |
| `Holding` | Quantity of one stock owned by a trader |
| `Portfolio` | All of a trader's holdings, plus their transaction history |
| `User` / `Trader` | Trader identity (`User`) and trading behavior (`Trader`, derived from `User`) |
| `Account` | The trader's cash balance — deposit, check funds, deduct/credit |
| `Market` | The list of tradable stocks; open/closed state; symbol lookup |
| `Order` | A single buy/sell request: validated, executed or rejected, status-tracked |
| `Transaction` | A record of one completed trade |
| `Simulation` | Starts/stops the simulation clock and drives price ticks |
| `MarketEvent` | A market-wide event (bull run, crash, etc.) and its price impact |
| `PriceAlert` | Watches one stock for a target price crossing |
| `Notification` | A message to the trader (currently: triggered price alerts) |

## Building

Requires a C++17 compiler (developed against g++ 13).

```bash
g++ -std=c++17 -Wall -Wextra -o simulator main.cpp
```

## Running

```bash
./simulator
```

You'll be asked for your name, email, 10-digit phone number, and starting
virtual balance to create your trader account. After that you'll see the
main menu:

```
1.  View Market
2.  Buy Stock
3.  Sell Stock
4.  View Portfolio
5.  View Balance
6.  Update Stock Price
7.  Account Summary
8.  Verify Details
9.  View Transaction History
10. View Order History
11. Set Price Alert
12. View Price Alerts
13. View Notifications
14. Mark All Notifications Read
15. Open Market (Admin)
16. Close Market (Admin)
17. Start Simulation (Admin)
18. Stop Simulation (Admin)
19. Advance Simulation Tick (Admin)
20. Exit
```

A typical session:

1. **View Market** (1) to see the available stocks and current prices.
2. **Buy Stock** (2) to purchase shares — this is rejected if you don't have
   enough virtual balance, or if the market is closed.
3. **Set Price Alert** (11) on a stock you're watching.
4. **Start Simulation** (17), then **Advance Simulation Tick** (19) a few
   times — each tick applies a random market event to every stock's price.
   If a tick crosses one of your alert targets, you'll see it fire
   immediately and it'll show up in **View Notifications** (13).
5. **View Portfolio** (4) and **Account Summary** (7) to see your updated
   holdings and balance as prices move.

Options 15/16/17/18/19 are administrative controls (opening/closing the
market, starting/stopping/advancing the simulation) — there's no separate
admin login yet, they're just exposed directly in the menu for now.

## Status

This is a work in progress being built incrementally, class by class, on top
of the initial OOP concepts demo. Planned next: integration with a real
market-data API, and a web-based UI.
