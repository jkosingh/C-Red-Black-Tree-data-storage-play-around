

#include "stocksystem.h"


// default constructor;
// begin with a balance of $100,000.00
StockSystem::StockSystem()
{
	balance = 100000;
}

// returns the balance member
double StockSystem::GetBalance()
{
	return balance;
}

// Add a new SKU to the system. Do not allow insertion of duplicate sku
bool StockSystem::StockNewItem(StockItem item)
{
	

	if (records.Search(item) == true)
	{
		return false;
	}

	//increment stock and insert new item into tree
	int number = item.GetStock() + 1;
	item.SetStock(number);
	return records.Insert(item);
}

// Locate the item with key itemsku and update its description field.
// Return false if itemsku is not found.
bool StockSystem::EditStockItemDescription(int itemsku, string desc)
{

	//dump the content of tree into an array to access the data members of records to change them
	int Asize = records.Size();
	StockItem *temp = records.Dump(Asize);
	for (int i = 0; i < Asize; i++)
	{
		if (temp[i].GetSKU() == itemsku)
		{
			StockItem *item = records.Retrieve(temp[i]); //item is a pointer to the data member of Stockitem class helps in channig values
			item->SetDescription(desc);
			delete[] temp;
			return true;
		}
	}
	delete[] temp;
	return false;
}

// Locate the item with key itemsku and update its description field.
// Return false if itemsku is not found or retailprice is negative.
bool StockSystem::EditStockItemPrice(int itemsku, double retailprice)
{
	if (retailprice < 0)
	{
		return false;
	}

	int Asize = records.Size();
	StockItem *temp = records.Dump(Asize);
	for (int i = 0; i < Asize; i++)
	{
		if (temp[i].GetSKU() == itemsku)
		{
			StockItem *item = records.Retrieve(temp[i]);//item is a pointer to the data member of Stockitem class helps in channig values
			item->SetPrice(retailprice);
			delete[] temp;
			return true;
		}
	}
	delete[] temp;
	return false;


}


// Purchase quantity of item at unitprice each, to reach a maximum (post-purchase) on-hand stock quantity of 1000.
// Return false if balance is not sufficient to make the purchase,
//   or if SKU does not exist, or if quantity or unitprice are negative.
// Otherwise, return true and increase the item's on-hand stock by quantity,
//   and reduce balance by quantity*unitprice.
bool StockSystem::Restock(int itemsku, int quantity, double unitprice)
{
	if (quantity < 0 || unitprice < 0 || unitprice*quantity > GetBalance())
	{
		return false;
	}


	int Asize = records.Size();
	StockItem *temp = records.Dump(Asize);
	for (int i = 0; i < Asize; i++)
	{
		if (temp[i].GetSKU() == itemsku)
		{
			StockItem *item = records.Retrieve(temp[i]);
			//when the stock is maxed out dont add anymore
			if(item->GetStock() == 1000 )
			{
				return false;
			}

			//purchase upto maximum stock of 1000
			if(item->GetStock() + quantity >= 1000)
			{
				quantity = 1000 - item->GetStock();

			}
			
			//increase the stock by quantity purchased
			int itembought = item->GetStock()+ quantity;
		    item->SetStock(itembought);
			balance = balance - unitprice*quantity;
			delete[] temp;
			return true;
		}

	}
	delete[] temp;
	return false;
}





// Sell an item to a customer, if quantity of stock is available and SKU exists.
// Reduce stock by quantity, increase balance by quantity*price, and return true if stock available.
// If partial stock (less than quantity) available, sell the available stock and return true.
// If no stock, sku does not exist, or quantity is negative, return false.
bool StockSystem::Sell(int itemsku, int quantity)
{
	if (quantity < 0 )
	{
		return false;

	}
	int Asize = records.Size();
	StockItem *temp = records.Dump(Asize);
	for (int i = 0; i < Asize; i++)
	{

		if (temp[i].GetSKU() == itemsku)
		{
			StockItem *item = records.Retrieve(temp[i]);//item is a pointer to the data member of Stockitem class helps in channig values
			
			//clear out stock if quantity if partial
			if (item->GetStock() <= quantity)
			{

				item->SetStock(0);
				balance +=item->GetStock()*item->GetPrice();
			}

			//reduce stock by quantity
			else
			{
				item->SetStock(item->GetStock() - quantity);
				balance += quantity*item->GetPrice();
			}
			delete[] temp;
			return true;
		}

	}
	delete[] temp;
	return false;
}






















