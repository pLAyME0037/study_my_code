{-# LANGUAGE DeriveFunctor #-}

import Secret
watUrName :: IO ()
watUrName = do
    putStrLn "What is Your Name?"
    name <- getLine
    putStrLn ("Hello, " ++ name)

watUrPureName :: World -> World
watUrPureName w1 = w4
    where w2          = printStr "What is your name?" w1
          (name, w3)  = readStr w2
          w4          = printStr ("Hello, " ++ name) w3

branch :: World -> (World, World)
branch w = (printStr "I Love You" w,
            printStr "I Hate You" w)

type WorldT a = World -> (a, World)
readStrT :: WorldT String
readStrT = readStr

printStrT :: String -> WorldT ()
printStrT s w = ((), printStr s w)

infixl 1 >>>=
(>>>=) :: WorldT a -> (a -> WorldT b) -> WorldT b
worldType >>>= f = uncurry f . worldType

watUrPureNameT :: WorldT ()
watUrPureNameT =
    printStrT "What is Your Name?" >>>= \_ ->
    readStrT                       >>>= \name ->
    printStrT ("Hello, " ++ name)

newtype WorldMonad a = WorldMonad { asT :: WorldT a } deriving Functor

instance Applicative WorldMonad where
    pure x = WorldMonad (\w -> (x, w))
    wtf <*> worldType = WorldMonad (asT wtf       >>>= \f ->
                                    asT worldType >>>= \x ->
                                    asT $ pure $ f x)

instance Monad WorldMonad where
    worldType >>= f = WorldMonad (asT worldType >>>= asT . f)

printStrM :: String -> WorldMonad ()
printStrM = WorldMonad . printStrT

readStrM :: WorldMonad String
readStrM = WorldMonad readStrT

watUrPureNameM :: WorldMonad ()
watUrPureNameM = do
    printStrM "What is your name?"
    name <- readStrM
    printStrM ("Hello, " ++ name)

main = do
    watUrName
    print $ watUrPureName World
    print $ branch World
    print $ watUrPureNameT World
    print $ asT watUrPureNameM World

