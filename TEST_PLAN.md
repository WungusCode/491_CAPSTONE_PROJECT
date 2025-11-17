# Budget Tracker - Test Plan (Sprint 2)

## Scope (in-scope features)
* Start Screen: Login, Create Account buttons, Exit
* Login Screen: Username and Password entries, Navigate back and forth, Create
* Home Page: 4 icon buttons (Pie Chart, Add/Delete Transactions, List transactions, Settings)
* Settings Page: Enable Notifications, Appearance, Account (Email and Password), Screentime Display, Sign Out button, Report Problem

## Out of Scope 
* COME BACK TO THIS 

## Test Types
* **Unit Testing** 
* **Integration/UI** 
* **DB Smoke**
* **Regression**: fix pointer issues and error outputs

## Risks
* GTK object lifetime can potentially lead to leaks 

## Environments
* Ubuntu 22.04, GTK 3.0
* Local run: 'make clean && make -B' && './sekai'
* CI: Github
