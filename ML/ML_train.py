import tensorflow as tf
import pandas as pd
import os
import numpy as np
from sklearn.neighbors import KNeighborsClassifier

def clean_data(file1, file2):

    # Reading and cleaning the data
    f = open(file1, "r")
    f2 = open(file2, "r")
    Lines = f.readlines()
    Lines2 = f2.readlines()

    f1_cleaned = file1[0:file1.find(".")] + "_cleaned.csv"
    f2_cleaned = file2[0:file2.find(".")] + "_cleaned.csv"

    # Check if the data is already cleaned
    if(os.path.exists(f1_cleaned) and os.path.exists(f2_cleaned)):
        print("Data already cleaned and saved as " + f1_cleaned + " and " + f2_cleaned)
    else:
        print("Cleaning data...")
        f = open(f1_cleaned, "w")
        f2 = open(f2_cleaned, "w")

        for line in Lines:
            if(line[0] == "C" or line[0] == "L"):
                f.write(line)
        for line in Lines2:
            if(line[0] == "C" or line[0] == "L"):
                f2.write(line)
        print("Data cleaned and saved as " + f1_cleaned + " and " + f2_cleaned)


    f.close()
    f2.close()

    

    # Read the data from the CSV file
    L1_data = pd.read_csv(f1_cleaned)
    L2_data = pd.read_csv(f2_cleaned)

    addressL1 = L1_data.iloc[:, 1]
    addressL2 = L2_data.iloc[:, 1]

    tagL1 = L1_data.iloc[:, 2]
    tagL2 = L2_data.iloc[:, 2]

    # Converting hexadecimal to decimal
    L1_data.iloc[:, 1] = addressL1.apply(lambda x: int(x, 16))
    L2_data.iloc[:, 1] = addressL2.apply(lambda x: int(x, 16))

    L1_data.iloc[:, 2] = tagL1.apply(lambda x: int(x, 16))  
    L2_data.iloc[:, 2] = tagL2.apply(lambda x: int(x, 16))

    # Print the first 5 rows of the dataframe
    # print("First 5 rows of the dataframe:")
    # print(L1_data.head())
    # print(L2_data.head())

    return L1_data, L2_data



dec = int(input("Enter 0 (to train) or 1 to test: "))

if(dec == 0):
    print("Training the model...")

    # Clean data
    file1 = "train_trial_L1.txt"
    file2 = "train_trial_L2.txt"
    L1_data, L2_data = clean_data(file1, file2)

    msk = np.random.rand(len(L1_data)) < 0.8
    train_data_L1 = L1_data[msk]
    inputData_train = (train_data_L1.iloc[:, 20:train_data_L1.shape[1] - 1]).to_numpy()
    outputData_train = (train_data_L1.iloc[:, train_data_L1.shape[1] - 1]).to_numpy()
    inputData_train = np.asarray(inputData_train).astype(np.float32)
    outputData_train = np.asarray(outputData_train).astype(np.float32)

    # print("Output data for L1: {}".format(outputData_train))
    # input("Pause")

    test_data_L1 = L1_data[~msk]
    inputData_test = (test_data_L1.iloc[:, 20:test_data_L1.shape[1] - 1]).to_numpy()
    outputData_test = (test_data_L1.iloc[:, test_data_L1.shape[1] - 1]).to_numpy()
    inputData_test = np.asarray(inputData_test).astype(np.float32)
    outputData_test = np.asarray(outputData_test).astype(np.float32)

    msk2 = np.random.rand(len(L2_data)) < 0.8
    train_data_L2 = L2_data[msk2]
    inputData_train2 = (train_data_L2.iloc[:, 1:train_data_L2.shape[1] - 1]).to_numpy()
    outputData_train2 = (train_data_L2.iloc[:, train_data_L2.shape[1] - 1]).to_numpy()
    inputData_train2 = np.asarray(inputData_train2).astype(np.float32)
    outputData_train2 = np.asarray(outputData_train2).astype(np.float32)

    test_data_L2 = L2_data[~msk2]
    inputData_test2 = (test_data_L2.iloc[:, 1:test_data_L2.shape[1] - 1]).to_numpy()
    outputData_test2 = (test_data_L2.iloc[:, test_data_L2.shape[1] - 1]).to_numpy()
    inputData_test2 = np.asarray(inputData_test2).astype(np.float32)
    outputData_test2 = np.asarray(outputData_test2).astype(np.float32)





    # Training model for L1 Cache
    print("Training model for L1 Cache...")

    # Define the input and output dimensions
    input_dim = inputData_train.shape[1]
    output_dim = 1

    print("Input dimensions for L1: {}".format(input_dim))
    print("Output dimensions for L1: {}".format(output_dim))


    # Define the model architecture
    model = tf.keras.Sequential()

    # Add the input layer
    model.add(tf.keras.layers.Dense(units=64, activation='tanh', input_shape=(input_dim,)))

    # Add additional hidden layers
    # model.add(tf.keras.layers.Dense(units=175, activation='tanh'))

    # Add the output layer
    model.add(tf.keras.layers.Dense(units=output_dim, activation='linear'))

    # Compile the model
    model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])

    # Train the model
    history = model.fit(inputData_train, outputData_train, epochs=1, batch_size=32, verbose=2)

    # Get the training history
    history.history

    # Test the model
    print("Testing the model for L1 Cache...")
    test_loss, test_accuracy = model.evaluate(inputData_test, outputData_test, verbose=2)
    print("Test accuracy: {}".format(test_accuracy))
    print("Test loss: {}".format(test_loss))


    # Predict
    predictions = model.predict(inputData_test[0,:].reshape(1, input_dim))
    print("Prediction: {} Actual: {}".format(predictions, outputData_test[0]))


    # Save the model
    model.save("model_L1.h5")

    # Print the model summary
    model.summary()

else:

    # load the model
    model = tf.keras.models.load_model("model_L1.h5")

    
