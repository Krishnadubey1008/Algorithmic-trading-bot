import pandas as pd
import yfinance as yf
from statsmodels.tsa.stattools import coint
import itertools

# --- Configuration ---
START_DATE = "2022-01-01" # Using a shorter period for a faster analysis
END_DATE = "2024-07-01"
COINTEGRATION_P_VALUE_THRESHOLD = 0.05 # We want p-values below this

def get_sp500_tickers():
    """Scrapes the list of S&P 500 tickers from Wikipedia."""
    print("Fetching S&P 500 ticker list from Wikipedia...")
    url = 'https://en.wikipedia.org/wiki/List_of_S%26P_500_companies'
    # pandas reads all tables from the page into a list of DataFrames
    tables = pd.read_html(url)
    sp500_table = tables[0] # The first table is the one we want
    # The ticker symbols are in the 'Symbol' column
    # Some tickers on Wikipedia have a '.' (e.g., BRK.B) which yfinance uses a '-' for
    tickers = sp500_table['Symbol'].str.replace('.', '-').tolist()
    print(f"Found {len(tickers)} tickers.")
    return tickers

def find_cointegrated_pairs(tickers):
    """
    Downloads data for a list of tickers and finds cointegrated pairs.
    """
    print(f"Downloading historical data for {len(tickers)} tickers...")
    # Download adjusted closing prices for all tickers
    data = yf.download(tickers, start=START_DATE, end=END_DATE, auto_adjust=True)['Close']
    
    # Drop any stocks that have missing data for our time period
    data = data.dropna(axis=1, how='any') 
    
    valid_tickers = data.columns.tolist()
    print(f"\nFinding cointegrated pairs from {len(valid_tickers)} tickers with complete data...")
    
    cointegrated_pairs = []
    
    # Get all unique combinations of 2 tickers
    ticker_combinations = list(itertools.combinations(valid_tickers, 2))
    
    # Loop through all pairs and test for cointegration
    for i, combo in enumerate(ticker_combinations):
        stock1 = data[combo[0]]
        stock2 = data[combo[1]]
        
        # Run the cointegration test
        score, p_value, _ = coint(stock1, stock2)
        
        if p_value < COINTEGRATION_P_VALUE_THRESHOLD:
            cointegrated_pairs.append({
                "pair": f"{combo[0]}/{combo[1]}",
                "p_value": p_value,
            })
        
        # Print progress
        if (i + 1) % 5000 == 0:
            print(f"Tested {i + 1} / {len(ticker_combinations)} pairs...")
            
    # Sort the pairs by the best p-value (lowest is best)
    sorted_pairs = sorted(cointegrated_pairs, key=lambda x: x['p_value'])
    
    return sorted_pairs

if __name__ == "__main__":
    # Get the S&P 500 list
    sp500_tickers = get_sp500_tickers()
    
    # Find the cointegrated pairs from that list
    top_pairs = find_cointegrated_pairs(sp500_tickers)
    
    print("\n--- Top 15 Cointegrated Pairs Found ---")
    if not top_pairs:
        print("No cointegrated pairs found with the given threshold.")
    else:
        # Print the best 15 pairs
        for pair_info in top_pairs[:15]:
            print(f"Pair: {pair_info['pair']:<12} | P-Value: {pair_info['p_value']:.6f}")
