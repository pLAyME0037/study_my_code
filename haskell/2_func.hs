add :: (Int) -> (Int) -> Int
add num1 num2 = num1 + num2

hello :: (String) -> String
hello name = "Hello, " ++ name ++ "."

calculate :: (Int) -> (Int) -> Int
-- calculate bottom top = sum $ filter (\n -> n `mod` 2 == 0) [bottom..top]
calculate bottom top = sum $ filter even [bottom..top]

main = do
    print $ add 9 2
    print $ hello "mena"
    print $ calculate 9 32
    print $ calculate 2 10
