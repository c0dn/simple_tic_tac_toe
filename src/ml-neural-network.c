#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "C:\Users\limxu\OneDrive\Documents\a SIT Y1.1\Group Project\simple_tic_tac_toe\include\neuralnetwork.h"  // Include neural network header


int main()
{
    NeuralNetwork nn;
    initialize_network(&nn);

    // Load dataset
    Sample dataset[TOTAL_SAMPLES];
    read_dataset(IMPORT_FILE, dataset);

    // Verify total samples read
    printf("\nTotal samples read: %d\n", TOTAL_SAMPLES);

    // Shuffle dataset with a fixed seed for reproducibility
    srand(RAND_STATE);
    shuffle_dataset(dataset, TOTAL_SAMPLES);

    // Split dataset into training and testing
    Sample train_data[TRAINING_SAMPLES];
    Sample test_data[TESTING_SAMPLES]; // Changed from `test`
    split_dataset(dataset, train_data, test_data);

    // Verify sizes of train and test datasets
    printf("Training samples: %d\n", sizeof(train_data) / sizeof(train_data[0]));
    printf("Testing samples: %d\n\n", sizeof(test_data) / sizeof(test_data[0]));

    // Initialize the inputs and targets arrays for training data
    double inputs[TRAINING_SAMPLES][INPUT_NODES];
    double targets[TRAINING_SAMPLES][OUTPUT_NODES];
    for (int i = 0; i < TRAINING_SAMPLES; i++)
    {
        for (int j = 0; j < INPUT_NODES; j++)
        {
            inputs[i][j] = train_data[i].input[j];
        }
        targets[i][0] = train_data[i].output[0];
    }

    // Initialize the inputs and targets arrays for test data
    double test_inputs[TESTING_SAMPLES][INPUT_NODES];
    double test_targets[TESTING_SAMPLES][OUTPUT_NODES];
    for (int i = 0; i < TESTING_SAMPLES; i++)
    {
        for (int j = 0; j < INPUT_NODES; j++)
        {
            test_inputs[i][j] = test_data[i].input[j];
        }
        test_targets[i][0] = test_data[i].output[0];
    }

    // Train the neural network
    train(&nn, inputs, targets, test_inputs, test_targets);

    // Print trained weights and biases
    printf("\nTrained Weights and Biases:\n");
    print_weights_and_biases(&nn);

    // Calculate and print training and testing scores
    print_train_and_test_scores(&nn, train_data, test_data);

    // Save the trained model to the specified file path (this will overwrite the file if it exists)
    save_model(&nn, EXPORT_FILEPATH);

    // printf("Model saved to '%s'\n", EXPORT_FILEPATH);

    return 0;
}

void initialize_network(NeuralNetwork *nn)
{
    for (int i = 0; i < HIDDEN_NODES; i++)
    {
        for (int j = 0; j < INPUT_NODES; j++)
        {
            nn->weights[i][j] = ((double)rand() / RAND_MAX) * 2 - 1;
        }
        nn->bias_hidden[i] = ((double)rand() / RAND_MAX) * 2 - 1;
    }

    for (int i = 0; i < OUTPUT_NODES; i++)
    {
        for (int j = 0; j < HIDDEN_NODES; j++)
        {
            nn->output_weights[i][j] = ((double)rand() / RAND_MAX) * 2 - 1;
        }
        nn->bias_output[i] = ((double)rand() / RAND_MAX) * 2 - 1;
    }

    // Initialize Adam-specific moment terms to 0
    for (int i = 0; i < HIDDEN_NODES; i++)
    {
        for (int j = 0; j < INPUT_NODES; j++)
        {
            nn->m_weights[i][j] = 0.0;
            nn->v_weights[i][j] = 0.0;
        }
        nn->m_bias_hidden[i] = 0.0;
        nn->v_bias_hidden[i] = 0.0;
    }

    for (int i = 0; i < OUTPUT_NODES; i++)
    {
        for (int j = 0; j < HIDDEN_NODES; j++)
        {
            nn->m_output_weights[i][j] = 0.0;
            nn->v_output_weights[i][j] = 0.0;
        }
        nn->m_bias_output[i] = 0.0;
        nn->v_bias_output[i] = 0.0;
    }
}

void forward_pass(NeuralNetwork *nn, double input[])
{
    for (int i = 0; i < HIDDEN_NODES; i++)
    {
        nn->hidden_layer[i] = nn->bias_hidden[i];
        for (int j = 0; j < INPUT_NODES; j++)
        {
            nn->hidden_layer[i] += nn->weights[i][j] * input[j];
        }
        // ReLU activation function for the hidden layer
        nn->hidden_layer[i] = nn->hidden_layer[i] > 0 ? nn->hidden_layer[i] : 0.0;
    }

    for (int i = 0; i < OUTPUT_NODES; i++)
    {
        nn->output_layer[i] = nn->bias_output[i];
        for (int j = 0; j < HIDDEN_NODES; j++)
        {
            nn->output_layer[i] += nn->output_weights[i][j] * nn->hidden_layer[j];
        }
        // sigmoid activation for the output layer
        nn->output_layer[i] = 1.0 / (1.0 + exp(-nn->output_layer[i])); // Sigmoid activation
    }
}

void backward_pass(NeuralNetwork *nn, double input[], double target[], int t)
{
    double output_error[OUTPUT_NODES];
    double hidden_error[HIDDEN_NODES];

    // Output layer error
    for (int i = 0; i < OUTPUT_NODES; i++)
    {
        output_error[i] = target[i] - nn->output_layer[i];
        for (int j = 0; j < HIDDEN_NODES; j++)
        {
            // Calculate gradient
            double grad = output_error[i] * nn->hidden_layer[j];

            // Update m and v for Adam
            nn->m_output_weights[i][j] = BETA1 * nn->m_output_weights[i][j] + (1 - BETA1) * grad;
            nn->v_output_weights[i][j] = BETA2 * nn->v_output_weights[i][j] + (1 - BETA2) * (grad * grad);

            // Bias correction
            double m_hat = nn->m_output_weights[i][j] / (1 - pow(BETA1, t));
            double v_hat = nn->v_output_weights[i][j] / (1 - pow(BETA2, t));

            // Update weights
            nn->output_weights[i][j] += LEARNING_RATE * (m_hat / (sqrt(v_hat) + EPSILON) - LAMBDA * nn->output_weights[i][j]);
        }

        // Bias for output layer
        double bias_grad = output_error[i];
        nn->m_bias_output[i] = BETA1 * nn->m_bias_output[i] + (1 - BETA1) * bias_grad;
        nn->v_bias_output[i] = BETA2 * nn->v_bias_output[i] + (1 - BETA2) * (bias_grad * bias_grad);

        double m_hat_bias = nn->m_bias_output[i] / (1 - pow(BETA1, t));
        double v_hat_bias = nn->v_bias_output[i] / (1 - pow(BETA2, t));

        nn->bias_output[i] += LEARNING_RATE * m_hat_bias / (sqrt(v_hat_bias) + EPSILON);
    }

    // Hidden layer error and update
    for (int i = 0; i < HIDDEN_NODES; i++)
    {
        hidden_error[i] = 0.0;
        for (int j = 0; j < OUTPUT_NODES; j++)
        {
            hidden_error[i] += output_error[j] * nn->output_weights[j][i];
        }

        for (int j = 0; j < INPUT_NODES; j++)
        {
            double grad = hidden_error[i] * input[j];

            nn->m_weights[i][j] = BETA1 * nn->m_weights[i][j] + (1 - BETA1) * grad;
            nn->v_weights[i][j] = BETA2 * nn->v_weights[i][j] + (1 - BETA2) * (grad * grad);

            double m_hat = nn->m_weights[i][j] / (1 - pow(BETA1, t));
            double v_hat = nn->v_weights[i][j] / (1 - pow(BETA2, t));

            nn->weights[i][j] += LEARNING_RATE * m_hat / (sqrt(v_hat) + EPSILON);
        }

        double bias_grad = hidden_error[i];
        nn->m_bias_hidden[i] = BETA1 * nn->m_bias_hidden[i] + (1 - BETA1) * bias_grad;
        nn->v_bias_hidden[i] = BETA2 * nn->v_bias_hidden[i] + (1 - BETA2) * (bias_grad * bias_grad);

        double m_hat_bias = nn->m_bias_hidden[i] / (1 - pow(BETA1, t));
        double v_hat_bias = nn->v_bias_hidden[i] / (1 - pow(BETA2, t));

        nn->bias_hidden[i] += LEARNING_RATE * m_hat_bias / (sqrt(v_hat_bias) + EPSILON);
    }
}

void train(NeuralNetwork *nn, double inputs[TRAINING_SAMPLES][INPUT_NODES],
                          double targets[TRAINING_SAMPLES][OUTPUT_NODES],
                          double test_inputs[TESTING_SAMPLES][INPUT_NODES],
                          double test_targets[TESTING_SAMPLES][OUTPUT_NODES])
{
    int t = 1;
    int patience_counter = 0;
    double best_test_loss = INFINITY;

    for (int epoch = 0; epoch < EPOCHS; epoch++)
    {
        double total_loss = 0.0;

        // Training phase
        for (int i = 0; i < TRAINING_SAMPLES; i++)
        {
            forward_pass(nn, inputs[i]);
            backward_pass(nn, inputs[i], targets[i], t++);
            total_loss += 0.5 * pow(targets[i][0] - nn->output_layer[0], 2);
        }

        total_loss /= TRAINING_SAMPLES; // Average training loss

        // Test phase
        double test_loss = 0.0;
        for (int i = 0; i < TESTING_SAMPLES; i++)
        {
            forward_pass(nn, test_inputs[i]);
            test_loss += 0.5 * pow(test_targets[i][0] - nn->output_layer[0], 2);
        }

        test_loss /= TESTING_SAMPLES; // Average test loss

        // Early stopping based on test loss
        if (test_loss < best_test_loss)
        {
            best_test_loss = test_loss;
            patience_counter = 0;
        }
        else
        {
            patience_counter++;
        }

        if (patience_counter >= PATIENCE)
        {
            printf("Early stopping at epoch %d\n", epoch);
            break;
        }

        if (epoch % 100 == 0)
        {
            printf("Epoch %d, Training Loss: %f, Test Loss: %f\n", epoch, total_loss, test_loss);
        }
    }
}

int predict(NeuralNetwork *nn, double input[])
{
    forward_pass(nn, input);
    return (nn->output_layer[0] >= 0.5) ? 1 : 0;
}

void calculate_confusion_matrix(Sample *data, NeuralNetwork *nn, int size)
{
    int TP = 0, FP = 0, TN = 0, FN = 0;

    for (int i = 0; i < size; i++)
    {
        int prediction = predict(nn, data[i].input);
        int actual = (int)data[i].output[0];

        if (prediction == 1 && actual == 1)
        {
            TP++; // True Positive
        }
        else if (prediction == 1 && actual == 0)
        {
            FP++; // False Positive
        }
        else if (prediction == 0 && actual == 0)
        {
            TN++; // True Negative
        }
        else if (prediction == 0 && actual == 1)
        {
            FN++; // False Negative
        }
    }

    // Print confusion matrix as a table with aligned formatting
    printf("\nConfusion Matrix:\n");
    printf("-------------------------------------------------\n");
    printf("|                 | Predicted Positive | Predicted Negative |\n");
    printf("-------------------------------------------------\n");
    printf("| Actual Positive |       %7d      |      %7d       |\n", TP, FN);
    printf("| Actual Negative |       %7d      |      %7d       |\n", FP, TN);
    printf("-------------------------------------------------\n");

    // Print metrics
    print_metrics(TP, FP, TN, FN);
}

void print_metrics(int TP, int FP, int TN, int FN)
{
    double accuracy = (TP + TN) / (double)(TP + FP + TN + FN);
    double precision = (TP + FP > 0) ? TP / (double)(TP + FP) : 0.0;
    double recall = (TP + FN > 0) ? TP / (double)(TP + FN) : 0.0;
    double f1_score = (precision + recall > 0) ? 2 * (precision * recall) / (precision + recall) : 0.0;

    printf("\nMetrics:\n");
    printf("Accuracy: %.2f%%\n", accuracy * 100);
    printf("Precision: %.2f%%\n", precision * 100);
    printf("Recall: %.2f%%\n", recall * 100);
    printf("F1 Score: %.2f\n", f1_score);
}

void print_train_and_test_scores(NeuralNetwork *nn, Sample train_data[], Sample test_data[])
{
    // Calculate and print metrics for the training data
    printf("\nTraining Scores:\n");
    calculate_confusion_matrix(train_data, nn, TRAINING_SAMPLES); // For training data

    // Calculate and print metrics for the testing data
    printf("\nTesting Scores:\n");
    calculate_confusion_matrix(test_data, nn, TESTING_SAMPLES); // For testing data
}

void read_dataset(const char *filename, Sample dataset[])
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char line[100];
    int index = 0;
    while (fgets(line, sizeof(line), file) && index < TOTAL_SAMPLES)
    {
        char *token = strtok(line, ",");
        for (int i = 0; i < INPUT_NODES; i++)
        {
            if (strcmp(token, "x") == 0)
            {
                dataset[index].input[i] = 1.0;
            }
            else if (strcmp(token, "o") == 0)
            {
                dataset[index].input[i] = -1.0;
            }
            else
            {
                dataset[index].input[i] = 0.0;
            }
            token = strtok(NULL, ",");
        }
        dataset[index].output[0] = (strcmp(token, "positive\n") == 0) ? 1.0 : 0.0;
        index++;
    }
    fclose(file);
}

void shuffle_dataset(Sample dataset[], int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        Sample temp = dataset[i];
        dataset[i] = dataset[j];
        dataset[j] = temp;
    }
}

void split_dataset(Sample dataset[], Sample train_data[], Sample test[])
{
    for (int i = 0; i < TRAINING_SAMPLES; i++)
    {
        train_data[i] = dataset[i]; // First 766 for training
    }
    for (int i = 0; i < TESTING_SAMPLES; i++)
    {
        test[i] = dataset[TRAINING_SAMPLES + i]; // Next 192 for testing
    }
}

void print_weights_and_biases(NeuralNetwork *nn)
{
    printf("Hidden Layer Weights:\n");
    for (int i = 0; i < HIDDEN_NODES; i++)
    {
        for (int j = 0; j < INPUT_NODES; j++)
        {
            printf("Weight[%d][%d]: %f\n", i, j, nn->weights[i][j]);
        }
    }

    printf("\nHidden Layer Biases:\n");
    for (int i = 0; i < HIDDEN_NODES; i++)
    {
        printf("Bias[%d]: %f\n", i, nn->bias_hidden[i]);
    }

    printf("\nOutput Layer Weights:\n");
    for (int i = 0; i < OUTPUT_NODES; i++)
    {
        for (int j = 0; j < HIDDEN_NODES; j++)
        {
            printf("Output Weight[%d][%d]: %f\n", i, j, nn->output_weights[i][j]);
        }
    }

    printf("\nOutput Layer Biases:\n");
    for (int i = 0; i < OUTPUT_NODES; i++)
    {
        printf("Output Bias[%d]: %f\n", i, nn->bias_output[i]);
    }
}



void save_model(NeuralNetwork *nn, const char *nn_weights)
{
    FILE *file = fopen(nn_weights, "wb");
    if (file == NULL)
    {
        printf("Failed to open file for saving model. Check if the path is correct and accessible.\n");
        return;
    }

    // Save weights and biases
    fwrite(nn->weights, sizeof(double), HIDDEN_NODES * INPUT_NODES, file);
    fwrite(nn->bias_hidden, sizeof(double), HIDDEN_NODES, file);
    fwrite(nn->output_weights, sizeof(double), OUTPUT_NODES * HIDDEN_NODES, file);
    fwrite(nn->bias_output, sizeof(double), OUTPUT_NODES, file);

    fclose(file);

    printf("Model saved successfully to '%s'.\n", nn_weights);
}
