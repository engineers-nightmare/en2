#pragma once

#include <vector>

#include "mechanic.h"
#include "tank.h"

class Combinator : public Mechanic {
public:
    using ptr = std::shared_ptr<Combinator>;
    static ptr create(std::string const& name = "Combinator");

protected:
    Tank::ptr tankProduct;
    std::vector<Tank::ptr> tanks;
    std::vector<uint> mixes;
    std::vector<double> mixRatios;

    LitersPerSecond mixRate;
    double sumMixes;

public:
    Combinator(std::string const& name);
    virtual void tick() override;
    virtual void dump(std::stringstream & stream) const override;

    void setTanks(uint num);

    /** get product tank */
    Tank::ptr const& getProductTank() const;

    /** get tank (_not_ product) at index */
    Tank::ptr const& getTank(size_t index) const;

    /** get tanks (_not_ product) */
    std::vector<Tank::ptr> const& getTanks() const;

    /**
     * set mix rate for tank, mix 0-100
     * given : mix at 100 ml
     * if two tanks, each set to 100, mix will be 50 from 1, 50 from 2
     * if 3 tanks, 2 at 50, 1 at 100, mix will be 25 from 1 & 2, 50 from 3
     */
    void setMix(size_t tank, uint mix);

    void setMixRate(LitersPerSecond m);
};
