#include <ql/quantlib.hpp>
#include <ql/settings.hpp>
#include <boost/shared_ptr.hpp>

#pragma warning(disable: 4819)

int main() {
	std::vector<QuantLib::Date> dates;
	std::vector<QuantLib::DiscountFactor> discountFactors;
	
	QuantLib::Date valuation_date(31, QuantLib::Dec, 2016);
	QuantLib::Settings::instance().evaluationDate() = valuation_date;
	
	dates.push_back(valuation_date);
	discountFactors.push_back(1.0);
	dates.push_back(QuantLib::Date(31, QuantLib::Dec, 2017));
	discountFactors.push_back(0.99);
	dates.push_back(QuantLib::Date(31, QuantLib::Dec, 2028));
	discountFactors.push_back(0.80);

	boost::shared_ptr<QuantLib::YieldTermStructure> forwardCurve(new QuantLib::InterpolatedDiscountCurve<QuantLib::LogLinear>(dates, discountFactors, QuantLib::Actual360()));
	//boost::shared_ptr<QuantLib::YieldTermStructure> tmpYieldTerm = boost::make_shared<QuantLib::InterpolatedDiscountCurve<QuantLib::LogLinear>>(dates, discountFactors, QuantLib::Actual360());
	//const QuantLib::Date tmpDate = QuantLib::Date(11, QuantLib::Nov, 2019);
	//const QuantLib::Compounding tmpCpd = QuantLib::Continuous;
	//std::cout << (*forwardCurve).discount(tmpDate) << std::endl;
	//QuantLib::InterestRate tmp = (*forwardCurve).zeroRate(tmpDate, QuantLib::Actual360(), tmpCpd, QuantLib::Annual, false);
	//std::cout << tmp << std::endl;

	discountFactors.pop_back(); discountFactors.pop_back();
	discountFactors.push_back(0.999); discountFactors.push_back(0.89);
	boost::shared_ptr<QuantLib::YieldTermStructure> oisCurve(new QuantLib::InterpolatedDiscountCurve<QuantLib::LogLinear>(dates, discountFactors, QuantLib::Actual360()));

	QuantLib::Handle<QuantLib::YieldTermStructure> forwardTermStructure(forwardCurve);
	QuantLib::Handle<QuantLib::YieldTermStructure> discountingTermStructure(oisCurve);
	
	QuantLib::Real nominal = 1000000.0;
	QuantLib::Date previousResetDate(20, QuantLib::Nov, 2016);
	QuantLib::Date maturity(20, QuantLib::Nov, 2026);
	double spread = 0.02;
	double fixRate = 0.04;
	boost::shared_ptr<QuantLib::IborIndex> euribor(new QuantLib::Euribor(3 * QuantLib::Months, forwardTermStructure));
	//boost::shared_ptr<QuantLib::IborIndex> tmp_euribor = boost::make_shared<QuantLib::Euribor>(3 * QuantLib::Months, forwardTermStructure);
	euribor->addFixing(euribor->fixingDate(previousResetDate), 0.01, true);   // I do not follow what the point of InterestRateIndex::fixingDate or Index::addFixing
	
	QuantLib::VanillaSwap::Type swapType = QuantLib::VanillaSwap::Payer;
	QuantLib::Schedule fixedSchedule(previousResetDate, maturity, 1 * QuantLib::Years, QuantLib::TARGET(), 
		QuantLib::ModifiedFollowing, QuantLib::ModifiedFollowing, QuantLib::DateGeneration::Forward, false);
	std::vector<QuantLib::Date> tmp = fixedSchedule.dates();
	for (auto i = tmp.begin(); i != tmp.end(); i++) {
		std::cout << *i << std::endl;
	}

	return 0;
}



//using namespace std;
//using namespace QuantLib;
//
//int main()
//{
//	vector<Date> dates;
//	vector<DiscountFactor> discountFactor;
//
//	Date valuationDate(31, December, 2012);
//	Settings::instance().evaluationDate() = valuationDate;
//
//	dates.push_back(valuationDate); discountFactor.push_back(1.0);
//	dates.push_back(Date(31, December, 2013));  discountFactor.push_back(0.99);
//	dates.push_back(Date(31, December, 2024));  discountFactor.push_back(0.80);
//	boost::shared_ptr<YieldTermStructure> forwardCurve(new InterpolatedDiscountCurve<LogLinear>(dates, discountFactor, Actual360()));
//
//	discountFactor.pop_back(); discountFactor.pop_back();
//	
//	discountFactor.push_back(0.999);
//	discountFactor.push_back(0.89);
//
//	boost::shared_ptr<YieldTermStructure> oisCurve(new InterpolatedDiscountCurve<LogLinear>(dates, discountFactor, Actual360()));
//
//	Handle<YieldTermStructure> discountingTermStructure(oisCurve);
//	Handle<YieldTermStructure> forwardingTermStructure(forwardCurve);
//
//	Real nominal = 1000000.0;
//	Date previousResetDate(20, November, 2012);
//	Date maturity(20, November, 2022);
//	double spread = 0.02;
//	double fixedRate = 0.04;
//
//	boost::shared_ptr<IborIndex> euribor(new Euribor(3 * Months, forwardingTermStructure));
//	euribor->addFixing(euribor->fixingDate(previousResetDate), 0.01, true);
//
//	VanillaSwap::Type swapType = VanillaSwap::Payer;
//
//	Schedule fixedSchedule(previousResetDate, maturity, 1 * Years,
//		TARGET(), ModifiedFollowing, ModifiedFollowing,
//		DateGeneration::Forward, false);
//
//	Schedule floatSchedule(previousResetDate, maturity, 3 * Months,
//		TARGET(), ModifiedFollowing, ModifiedFollowing,
//		DateGeneration::Forward, false);
//
//	VanillaSwap swap(VanillaSwap::Payer, nominal, fixedSchedule, fixedRate, Thirty360(), floatSchedule, euribor, spread, Actual360());
//
//	boost::shared_ptr<PricingEngine> swapEngine(new DiscountingSwapEngine(discountingTermStructure));
//
//	swap.setPricingEngine(swapEngine);
//
//	double res = swap.NPV();
//
//	return 0;
//}