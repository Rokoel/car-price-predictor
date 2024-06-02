#include <vector>
#include <QStringList>


struct Row {
    QString brandName, modelName;
    int year, odometer;
    double price;

    Row(QString brandName_, QString modelName_, int year_, int odometer_, double price_)
        : brandName(brandName_)
        , modelName(modelName_)
        , year(year_)
        , odometer(odometer_)
        , price(price_) {}

    bool sameCar(Row other) {
        return brandName == other.brandName && modelName == other.modelName;
    }

    double diff(Row other) {
        return abs(year - other.year) + abs(odometer - other.odometer);
    }
};

class Predictor {
    std::vector <Row> cars;
    int lstLen = 5;

public:
    Predictor() {
        std::vector <Row> c;
        cars = c;
    }

    void feedRow(const QString brandName_, const QString modelName_, const int year_, const int odometer_, const int price_) {
        Row newRow(brandName_, modelName_, year_, odometer_, price_);
        cars.push_back(newRow);
    }

    void setLstLen(int newVal) {
        lstLen = newVal;
    }

    double fit(const QString brandName_, const QString modelName_, const int year_, const int odometer_) const {
        Row toFit(brandName_, modelName_, year_, odometer_, 0);
        std::vector <Row> comp;
        for (Row r: cars) {
            if (comp.size() < lstLen) {
                comp.push_back(r);
                continue;
            }
            for (int i = 0; i < lstLen; i++) {
                if (toFit.sameCar(comp[i]) && !toFit.sameCar(r)) {
                    continue;
                }
                if (!toFit.sameCar(comp[i]) && toFit.sameCar(r)) {
                    for (int j = i + 1; j < lstLen; j++) {
                        comp[j] = comp[j - 1];
                    }
                    comp[i] = r;
                    break;
                }
                if (toFit.diff(comp[i]) > toFit.diff(r)) {
                    for (int j = i + 1; j < lstLen; j++) {
                        comp[j] = comp[j - 1];
                    }
                    comp[i] = r;
                    break;
                }
            }
        }
        double ans = 0;
        for (Row r : comp) {
            ans += r.price;
        }
        return ans / lstLen;
    }
};
